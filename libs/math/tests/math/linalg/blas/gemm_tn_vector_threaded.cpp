#include <gtest/gtest.h>

#include "math/linalg/blas/base.hpp"
#include "math/linalg/blas/gemm_tn_vector.hpp"
#include "math/linalg/blas/gemm_tn_vector_threaded.hpp"
#include "math/linalg/matrix.hpp"
#include "math/linalg/prototype.hpp"

using namespace fetch;
using namespace fetch::math::linalg;

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded1)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(1), beta = double(0);

  Matrix<double> A = Matrix<double>(R"(
	0.3745401188473625 0.9507143064099162 0.7319939418114051;
 0.5986584841970366 0.15601864044243652 0.15599452033620265
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.05808361216819946 0.8661761457749352 0.6011150117432088;
 0.7080725777960455 0.020584494295802447 0.9699098521619943
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.8324426408004217 0.21233911067827616 0.18182496720710062;
 0.18340450985343382 0.3042422429595377 0.5247564316322378;
 0.43194501864211576 0.2912291401980419 0.6118528947223795
	)");

  Matrix<double> R = Matrix<double>(R"(
	0.4456482991294304 0.33674079873438223 0.8057864498423064;
 0.1656934419785827 0.8266976184734581 0.7228126579480724;
 0.15297229436215787 0.6372467595628419 0.591313169085288
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded2)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(0), beta = double(1);

  Matrix<double> A = Matrix<double>(R"(
	0.13949386065204183 0.29214464853521815 0.3663618432936917;
 0.45606998421703593 0.7851759613930136 0.19967378215835974
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.5142344384136116 0.5924145688620425 0.046450412719997725;
 0.6075448519014384 0.17052412368729153 0.06505159298527952
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.9488855372533332 0.9656320330745594 0.8083973481164611;
 0.3046137691733707 0.09767211400638387 0.6842330265121569;
 0.4401524937396013 0.12203823484477883 0.4951769101112702
	)");

  Matrix<double> R = Matrix<double>(R"(
	0.9488855372533332 0.9656320330745594 0.8083973481164611;
 0.3046137691733707 0.09767211400638387 0.6842330265121569;
 0.4401524937396013 0.12203823484477883 0.4951769101112702
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded3)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(1), beta = double(1);

  Matrix<double> A = Matrix<double>(R"(
	0.034388521115218396 0.9093204020787821 0.2587799816000169;
 0.662522284353982 0.31171107608941095 0.5200680211778108
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.5467102793432796 0.18485445552552704 0.9695846277645586;
 0.7751328233611146 0.9394989415641891 0.8948273504276488
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.5978999788110851 0.9218742350231168 0.0884925020519195;
 0.1959828624191452 0.045227288910538066 0.32533033076326434;
 0.388677289689482 0.2713490317738959 0.8287375091519293
	)");

  Matrix<double> R = Matrix<double>(R"(
	1.1302433056071457 1.5506700912834535 0.7146781438045392;
 0.9347351599342958 0.5061714427949007 1.4859210106475778;
 0.9332767593138604 0.8077890198114085 1.545017690717192
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded4)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(0.21996470889077302), beta = double(0.5810464525320076);

  Matrix<double> A = Matrix<double>(R"(
	0.3567533266935893 0.28093450968738076 0.5426960831582485;
 0.14092422497476265 0.8021969807540397 0.07455064367977082
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.9868869366005173 0.7722447692966574 0.1987156815341724;
 0.005522117123602399 0.8154614284548342 0.7068573438476171
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.7290071680409873 0.7712703466859457 0.07404465173409036;
 0.3584657285442726 0.11586905952512971 0.8631034258755935;
 0.6232981268275579 0.3308980248526492 0.06355835028602363
	)");

  Matrix<double> R = Matrix<double>(R"(
	0.5012023237840982 0.5340223358191966 0.08052864171676313;
 0.27024499224399195 0.2589389618440707 0.6385114844951468;
 0.4800643472614559 0.297825412635623 0.07224324460786048
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded5)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(0.9445218083615292), beta = double(-0.18280419873856468);

  Matrix<double> A = Matrix<double>(R"(
	0.3109823217156622 0.32518332202674705 0.7296061783380641;
 0.6375574713552131 0.8872127425763265 0.4722149251619493
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.1195942459383017 0.713244787222995 0.7607850486168974;
 0.5612771975694962 0.770967179954561 0.49379559636439074
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.5227328293819941 0.42754101835854963 0.02541912674409519;
 0.10789142699330445 0.03142918568673425 0.6364104112637804;
 0.3143559810763267 0.5085706911647028 0.907566473926093
	)");

  Matrix<double> R = Matrix<double>(R"(
	0.27756440791032266 0.5956111656594916 0.5161756104882738;
 0.48735517856884836 0.8593867332330396 0.5311277613421683;
 0.2752896009588596 0.7424134676153771 0.5786134135425791
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded6)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(3), beta = double(3);

  Matrix<double> A = Matrix<double>(R"(
	0.24929222914887494 0.41038292303562973 0.7555511385430487;
 0.22879816549162246 0.07697990982879299 0.289751452913768
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.16122128725400442 0.9296976523425731 0.808120379564417;
 0.6334037565104235 0.8714605901877177 0.8036720768991145
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.18657005888603584 0.8925589984899778 0.5393422419156507;
 0.8074401551640625 0.8960912999234932 0.3180034749718639;
 0.11005192452767676 0.22793516254194168 0.4271077886262563
	)");

  Matrix<double> R = Matrix<double>(R"(
	1.1150486714307748 3.9711419490303896 2.7740372088277763;
 2.7670859470329567 4.0341248930495945 2.1345266475413536;
 1.2461775311992485 3.54862876330246 3.8116476403012087
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}

TEST(blas_gemm_vectorised, blas_gemm_tn_vector_threaded7)
{

  Blas<double, Signature(_C <= _alpha, _A, _B, _beta, _C),
       Computes(_C = _alpha * T(_A) * _B + _beta * _C),
       platform::Parallelisation::VECTORISE | platform::Parallelisation::THREADING>
      gemm_tn_vector_threaded;
  // Compuing _C =  _alpha * T(_A) * _B + _beta * _C

  double alpha = double(1.5869784631572492), beta = double(2.316683831741167);

  Matrix<double> A = Matrix<double>(R"(
	0.8180147659224931 0.8607305832563434 0.006952130531190703 0.5107473025775657 0.417411003148779;
 0.22210781047073025 0.1198653673336828 0.33761517140362796 0.9429097039125192 0.32320293202075523
	)");

  Matrix<double> B = Matrix<double>(R"(
	0.5187906217433661 0.7030189588951778 0.363629602379294 0.9717820827209607 0.9624472949421112 0.25178229582536416 0.49724850589238545;
 0.30087830981676966 0.2848404943774676 0.036886947354532795 0.6095643339798968 0.5026790232288615 0.05147875124998935 0.27864646423661144
	)");

  Matrix<double> C = Matrix<double>(R"(
	0.9082658859666537 0.23956189066697242 0.1448948720912231 0.489452760277563 0.9856504541106007 0.2420552715115004 0.6721355474058786;
 0.7616196153287176 0.23763754399239967 0.7282163486118596 0.3677831327192532 0.6323058305935795 0.6335297107608947 0.5357746840747585;
 0.0902897700544083 0.835302495589238 0.32078006497173583 0.18651851039985423 0.040775141554763916 0.5908929431882418 0.6775643618422824;
 0.016587828927856152 0.512093058299281 0.22649577519793795 0.6451727904094499 0.17436642900499144 0.690937738102466 0.3867353463005374;
 0.9367299887367345 0.13752094414599325 0.3410663510502585 0.11347352124058907 0.9246936182785628 0.877339353380981 0.2579416277151556
	)");

  Matrix<double> R = Matrix<double>(R"(
	2.8836979369591873 1.568029223653152 0.8207312314729389 2.6103068337322943 3.710046882327716 0.9057674604100405 2.3008569439156337;
 2.530313754751488 1.5650108933578513 2.1907676328092114 2.295407181134018 2.8751395580100425 1.8214052902455173 1.973447740938842;
 0.3761035954923789 2.0955022314269964 0.7669214563160354 0.7694232327964352 0.37441106950058034 1.3992717094651925 1.7244839192600723;
 0.9091593844552449 2.182415065311832 0.8746540393692535 3.1944727394638366 1.9362562262121077 1.8817967994240568 1.7159460685538264;
 2.668091009050249 0.9303872373555095 1.0499391574018413 1.2192677236063314 3.0376014391463384 2.2257083753789866 1.0698799392050833
	)");

  gemm_tn_vector_threaded(alpha, A, B, beta, C);

  ASSERT_TRUE(R.AllClose(C));
}
