TEST(OtherTests, Test1) {
            const char* input = "100 200 300 -400 0 00010";
            std::stringstream stream(input);
            std::stringstream stream_copy(input); 
            while (!stream.eof() && !stream_copy.eof()) {
                BigInteger big;
                int regular;
                stream >> big;
                stream_copy >> regular;
                ASSERT_TRUE( big == regular );
            }
}

TEST(OtherTests, Test2) {
            std::stringstream output_big;
            std::stringstream output_reg;
            for (auto item : {-1, 0, 1, 2, 3, 100, -100}) {
                output_reg << item;
                BigInteger bigint(item);
                ASSERT_TRUE(item == bigint);
                ASSERT_TRUE(!(item + 1 == bigint));
                output_big << bigint;
                ASSERT_TRUE(std::to_string(item) == bigint.toString());
            }
            ASSERT_TRUE(output_big.str() == output_reg.str());
}
        
TEST(OtherTests, Test3) {
            std::vector<std::vector<int>> sum_tests = {
                {2, 2},
                {0, 0, 0},
                {-2, -2},
                {-2, 2},
                {1, 1, 1, 1},
                std::vector<int>(100, 100),
            };
            for (const auto& items : sum_tests) {
                int regular = std::accumulate(items.begin(), items.end(), 0);
                BigInteger big = std::accumulate(items.begin(), items.end(), 0_bi, [](const auto& left, const auto& right) {
                    return left + right;        
                });
                ASSERT_TRUE(big == regular);
            }
            BigInteger bigint = 0;
            bigint += 1;
            bigint += -2;
            ASSERT_TRUE(bigint == -1);

            int big_number = std::numeric_limits<int>::max() - 1;
            bigint = big_number;
            bigint += 2;
            ASSERT_TRUE(big_number < bigint);
        }

TEST(OtherTests, Test4){
            std::vector<std::array<int, 2>> substract_tests = {
                {2, 2},
                {0, 0},
                {-2, -2},
                {-2, 2},
                {1, 100},
                {100, 1}
            };

            for (const auto& items : substract_tests) {
                int regular = items[0] - items[1];
                BigInteger big = BigInteger(items[0]) - BigInteger(items[1]);
                ASSERT_TRUE(big == regular);
            }
            BigInteger bigint = 0;
            bigint -= 1;
            bigint -= -2;
            ASSERT_TRUE(bigint == 1);

            int small_number = std::numeric_limits<int>::min();
            bigint = small_number;
            bigint -= 2;
            ASSERT_TRUE(small_number > bigint);
        }
TEST(OtherTests, Test5) {
            BigInteger bigint = 0_bi * 1_bi;       
            ASSERT_TRUE(bigint == 0_bi);   
            ASSERT_TRUE(bigint == 0);   

            bigint = 1;
            ASSERT_TRUE(bigint == 1);   
            bigint *= 1;
            ASSERT_TRUE(bigint == 1);   
            bigint *= 2;
            ASSERT_TRUE(bigint == 2);   
            bigint *= 2;
            ASSERT_TRUE(bigint == 4);   
            bigint *= -2;
            ASSERT_TRUE(bigint == -8);   
}

TEST(OtherTests, Test6) {
            BigInteger bigint = 1;
            ASSERT_TRUE( 0_bi /  bigint == 0);
            bigint = 10;
            bigint /= 2;
            ASSERT_TRUE(bigint == 5);
            ASSERT_TRUE(bigint % 1 == 0);
            ASSERT_TRUE(bigint % 3 == 2);
            ASSERT_TRUE(bigint / 4 == 1);
}
TEST(OtherTests, Test7) {
            BigInteger bigint = 0;       
            ASSERT_TRUE((--bigint) == -1);
            ASSERT_TRUE((bigint--) == -1);
            ASSERT_TRUE(bigint == -2);

            ASSERT_TRUE((++bigint) == -1);
            ASSERT_TRUE((bigint++) == -1);
            ASSERT_TRUE(bigint == 0);

            ASSERT_TRUE(-bigint == bigint);
            bigint = 1;
            ASSERT_TRUE(-bigint != bigint);
            ASSERT_TRUE(-bigint == -1);
            ASSERT_TRUE(bigint == 1);
}


TEST(OtherTests, Test8) {
            Rational rational(10);
            Rational rational2(10_bi);
            ASSERT_TRUE(rational == rational2);
            ASSERT_TRUE(rational / rational2 == 1);
            ASSERT_TRUE((rational2 /= 5) == 2);
            ASSERT_TRUE((rational2 /= 5) < 1);
            ASSERT_TRUE(rational2 > 0);
            ASSERT_TRUE((rational2 += 1) > 1);
            ASSERT_TRUE(rational2.toString() == "7/5");
            rational2 -= 2;
            ASSERT_TRUE(rational2.toString() == "-3/5");
            const double acceptable_error = 0.01;
            ASSERT_TRUE(std::abs(double(rational2) - -0.6) < acceptable_error);
            ASSERT_TRUE(std::abs(double(-rational2) - 0.6) < acceptable_error);
            rational2 += Rational(3) / Rational(5);
            ASSERT_TRUE(rational2 == 0);
            ASSERT_TRUE(rational2.toString() == "0");
            ASSERT_TRUE(rational2.asDecimal(10) == "0.0000000000");
            rational2 += 1;
            ASSERT_TRUE(rational2.toString() == "1");
            ASSERT_TRUE(rational2.asDecimal(5) == "1.00000");
            rational2 /= 2;
            ASSERT_TRUE(rational2.asDecimal(2) == "0.50");
            std::cout << rational2.asDecimal(2) << '\n';
}
