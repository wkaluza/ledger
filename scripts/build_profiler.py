import argparse
import json
import os
import shutil
import subprocess
import sys
import time
import uuid  # ???

from os.path import abspath, isdir, isfile, join, relpath


def tool_is_compiler(config):
    build_tool = config['build_cmd'][0]

    c_compiler = shutil.which(os.environ['CC'])
    cpp_compiler = shutil.which(os.environ['CXX'])

    return build_tool in (c_compiler, cpp_compiler)


def source_file(config):
    build_action = config['build_action']
    sources_root = config['source_dir']
    toolchain_cmd = config['build_cmd']

    if tool_is_compiler(config):
        if build_action == 'COMPILE':
            src_file = relpath(abspath(
                join(os.getcwd(), toolchain_cmd[-1])), abspath(sources_root))

            return {'source_file': src_file}

    return {}


def process(config):
    raw_data = []
    for root, dirs, files in os.walk(config['profile_data_root']):
        for file in files:
            with open(join(root, file), 'r') as f:
                raw_data.append(json.load(f))

    assert raw_data

    projects = set()
    targets = set()
    earliest_start = None
    for raw in raw_data:
        projects.add(raw['project_name'])
        targets.add(raw['target_name'])
        earliest_start = raw['start_ms'] if earliest_start is None else min(
            earliest_start, raw['start_ms'])
    sorted_projects = sorted(list(projects))
    sorted_targets = sorted(list(targets))

    assert sorted_projects
    assert sorted_targets

    trace_events = []
    for raw in raw_data:
        event = {
            'ph': 'X',
            'ts': (raw['start_ms'] - earliest_start)
                  * 1000, 'dur': raw['duration_ms'] * 1000}
        if 'source_file' in raw:
            event['name'] = raw['source_file']
        else:
            # ???aggregate link statics
            event['name'] = raw['target_name']

        event['cat'] = ','.join(
            list(set([raw['build_action'], raw['project_name'], raw['target_name'], raw['target_type']])))
        event['pid'] = 0  # raw['target_name']
        event['tid'] = 1
        trace_events.append(event)

    # ???inefficient
    for target in sorted_targets:
        earliest_start_ms = None
        latest_end_ms = None
        for raw in raw_data:
            if raw['target_name'] == target:
                earliest_start_ms = (raw['start_ms'] - earliest_start) if earliest_start_ms is None else min(
                    raw['start_ms'] - earliest_start, earliest_start_ms)
                latest_end_ms = (
                    raw['start_ms'] - earliest_start + raw['duration_ms']) if latest_end_ms is None else max(
                    raw['start_ms'] - earliest_start + raw['duration_ms'], latest_end_ms)

        event = {'name': target, 'ph': 'X', 'ts': earliest_start_ms
                 * 1000, 'dur': (latest_end_ms - earliest_start_ms) * 1000,
                 'pid': 0,  # target,
                 'tid': 0
                 }
        event['cat'] = ','.join(list(set([target, 'TARGET_EVENT'])))
        trace_events.append(event)

    tef_data = {
        'traceEvents': trace_events,
        'displayTimeUnit': 'ms'
    }
    print(json.dumps(tef_data, indent=2, sort_keys=True))


def save_as_json(config, data):
    build_action = data['build_action']
    project_name = data['project_name']

    profile_data_root = config['profile_data_root']
    if not isdir(profile_data_root):
        os.mkdir(profile_data_root)

    path_to_project_profiling_dir = join(
        profile_data_root, project_name)
    if not isdir(path_to_project_profiling_dir):
        os.mkdir(path_to_project_profiling_dir)

    name = data['target_name'] if build_action == 'LINK' else data['source_file']
    output_file_name = '{build_action}_{name}{uuid}.json'.format(build_action=build_action, name=name.replace(
        '/', '_').replace('.', '_'), uuid=('_' + str(uuid.uuid4()).replace('-', '')))

    output_path = join(path_to_project_profiling_dir,
                       output_file_name)

    assert not isfile(output_path), \
        'Old data found: only perform profiling on a clean build'

    print(json.dumps(data, indent=2, sort_keys=True))

    with open(output_path, 'w+') as f:
        json.dump(data, f, indent=2, sort_keys=True)


def run_build_cmd(config):
    start_ms = time.time() * 1000
    subprocess.run(config['build_cmd'])
    end_ms = time.time() * 1000

    duration_ms = end_ms - start_ms

    return start_ms, duration_ms


def parse_cli():
    build_cmd = None
    if '--' in sys.argv:
        separator_index = sys.argv.index('--')
        build_cmd = sys.argv[separator_index + 1:]

    parser = argparse.ArgumentParser()
    parser.add_argument('--mode', default='PROCESS')
    parser.add_argument('--profile_data_root')
    parser.add_argument('--source_dir')
    parser.add_argument('--project_name')
    parser.add_argument('--target_name')
    parser.add_argument('--target_type')
    parser.add_argument('--build_action')

    parsed, _ = parser.parse_known_args()

    mode = parsed.mode
    # ???enforce using argparse
    assert mode == 'COLLECT' or mode == 'PROCESS'

    build_action = parsed.build_action
    # ???enforce using argparse

    if mode == 'COLLECT':
        assert build_action == 'COMPILE' or build_action == 'LINK'
        assert build_cmd is not None  # enforce with argparse
    if mode == 'PROCESS':
        assert parsed.profile_data_root  # enforce with argparse

    return {'mode': mode, 'build_action': build_action, 'source_dir': parsed.source_dir,
            'project_name': parsed.project_name, 'target_name': parsed.target_name, 'target_type': parsed.target_type,
            'build_cmd': build_cmd, 'profile_data_root': parsed.profile_data_root}


def main(config):
    if config['mode'] == 'PROCESS':
        process(config)
    elif config['mode'] == 'COLLECT':
        start_ms, duration_ms = run_build_cmd(config)

        save_as_json(config, {**{
            'start_ms': start_ms,
            'duration_ms': duration_ms,
            'project_name': config['project_name'],
            'target_name': config['target_name'],
            'target_type': config['target_type'],
            'build_action': config['build_action']
        }, **source_file(config)})


if __name__ == '__main__':
    config = parse_cli()
    main(config)
    # implement queue for multiworker builds???
    # ???reintroduce master target and project phases.disregard projects for pids?
