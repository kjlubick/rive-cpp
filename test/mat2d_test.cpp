#include <catch.hpp>
#include "rive/math/mat2d.hpp"
#include "rive/math/math_types.hpp"

namespace rive
{
// Check Mat2D::findMaxScale.
TEST_CASE("findMaxScale", "[Mat2D]")
{
    Mat2D identity;
    // CHECK(1 == identity.getMinScale());
    CHECK(1 == identity.findMaxScale());
    // success = identity.getMinMaxScales(scales);
    // CHECK(success && 1 == scales[0] && 1 == scales[1]);

    Mat2D scale = Mat2D::fromScale(2, 4);
    // CHECK(2 == scale.getMinScale());
    CHECK(4 == scale.findMaxScale());
    // success = scale.getMinMaxScales(scales);
    // CHECK(success && 2 == scales[0] && 4 == scales[1]);

    Mat2D transpose = Mat2D(0,
                            3,
                            6,
                            0,
                            std::numeric_limits<float>::quiet_NaN(),
                            std::numeric_limits<float>::infinity());
    CHECK(transpose.findMaxScale() == 6);

    Mat2D rot90Scale = Mat2D::fromRotation(math::PI / 2);
    rot90Scale = Mat2D::fromScale(1.f / 4, 1.f / 2) * rot90Scale;
    // CHECK(1.f / 4 == rot90Scale.getMinScale());
    CHECK(1.f / 2 == rot90Scale.findMaxScale());
    // success = rot90Scale.getMinMaxScales(scales);
    // CHECK(success && 1.f / 4 == scales[0] && 1.f / 2 == scales[1]);

    Mat2D rotate = Mat2D::fromRotation(128 * math::PI / 180);
    // CHECK(math::nearly_equal(1, rotate.getMinScale(), math::EPSILON));
    CHECK(math::nearly_equal(1, rotate.findMaxScale(), math::EPSILON));
    // success = rotate.getMinMaxScales(scales);
    // CHECK(success);
    // CHECK(math::nearly_equal(1, scales[0], math::EPSILON));
    // CHECK(math::nearly_equal(1, scales[1], math::EPSILON));

    Mat2D translate = Mat2D::fromTranslate(10, -5);
    // CHECK(1 == translate.getMinScale());
    CHECK(1 == translate.findMaxScale());
    // success = translate.getMinMaxScales(scales);
    // CHECK(success && 1 == scales[0] && 1 == scales[1]);

    // skbug.com/4718
    Mat2D big(2.39394089e+36f,
              3.9159619e+36f,
              8.85347779e+36f,
              1.44823453e+37f,
              9.26526204e+36f,
              1.51559342e+37f);
    CHECK(big.findMaxScale() == 0);

    // // skbug.com/4718
    // Mat2D givingNegativeNearlyZeros(0.00436534f,
    //                                 0.00358857f,
    //                                 0.114138f,
    //                                 0.0936228f,
    //                                 0.37141f,
    //                                 -0.0174198f);
    // success = givingNegativeNearlyZeros.getMinMaxScales(scales);
    // CHECK(success && 0 == scales[0]);

    Mat2D baseMats[] = {scale, rot90Scale, rotate, translate};
    Mat2D mats[2 * std::size(baseMats)];
    for (size_t i = 0; i < std::size(baseMats); ++i)
    {
        mats[i] = baseMats[i];
        bool invertible = mats[i].invert(&mats[i + std::size(baseMats)]);
        REQUIRE(invertible);
    }
    srand(0);
    for (int m = 0; m < 1000; ++m)
    {
        Mat2D mat;
        for (int i = 0; i < 4; ++i)
        {
            int x = rand() % std::size(mats);
            mat = mats[x] * mat;
        }

        // float minScale = mat.findMinScale();
        float maxScale = mat.findMaxScale();
        // REQUIRE(minScale >= 0);
        REQUIRE(maxScale >= 0);

        // test a bunch of vectors. All should be scaled by between minScale and maxScale
        // (modulo some error) and we should find a vector that is scaled by almost each.
        static const float gVectorScaleTol = (105 * 1.f) / 100;
        static const float gCloseScaleTol = (97 * 1.f) / 100;
        float max = 0, min = std::numeric_limits<float>::max();
        Vec2D vectors[1000];
        for (size_t i = 0; i < std::size(vectors); ++i)
        {
            vectors[i].x = rand() * 2.f / RAND_MAX - 1;
            vectors[i].y = rand() * 2.f / RAND_MAX - 1;
            vectors[i] = vectors[i].normalized();
            vectors[i] = {mat[0] * vectors[i].x + mat[2] * vectors[i].y,
                          mat[1] * vectors[i].x + mat[3] * vectors[i].y};
        }
        for (size_t i = 0; i < std::size(vectors); ++i)
        {
            float d = vectors[i].length();
            REQUIRE(d / maxScale < gVectorScaleTol);
            // REQUIRE(minScale / d < gVectorScaleTol);
            if (max < d)
            {
                max = d;
            }
            if (min > d)
            {
                min = d;
            }
        }
        REQUIRE(max / maxScale >= gCloseScaleTol);
        // REQUIRE(minScale / min >= gCloseScaleTol);
    }
}
} // namespace rive
