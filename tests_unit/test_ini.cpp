#include <SimpleAPI.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

//========================================================================================
/*ПРИМЕР:
; простой комментарий
key = value

[group_1] ; строго БЕЗ пробелов!
key = value         ; даст итоговый объект { key : value }
key = key1 = value  ; даст итоговый объект { key : value, key1 : value }

[group_2]
key\inner_key\inner_inner_key = inner_inner_value ; вложенные значения и группы значений
=> даст итоговый объект
    { key : { inner_key : { inner_inner_key : inner_inner_value } } }

key\key1\key2 = a
key\key1\key2 = b
=> даст итоговый объект
    {
        key : {
            key1 : {
                key2 : [
                    a,
                    b
                ]
            }
        }
    }

key\key1\key2 = a
key\key1\key22 = b
=> даст итоговый объект
    {
        key : {
            key1 : {
                key2 : a,
                key22 : b
            }
        }
    }

; строка, число, отсутсвие значения (NULL), bool

key = simple string
key = many \
    line \
    string ; пробелы от начала строки до первого не пустого символа будут удалены при чтении

key[0] = value
key[2] = value
=> даст итоговый объект
    { key : [value, null, value]}

*/
//========================================================================================
std::string ini_example_string = std::string(
    "\n"
    "; комментарий\n"
    "; вторая строка комментария\n"
    "key =  15\n"
    "array = [a, 15, true]\n"
    "json = {\n"
    " inner_array:[\n"
    " a, 4.0, \"asd string\"]}\n"
    "\n"
    "[group 1]; ещё комментарий\n"
    "g1_key : value\n"
    "g1_key2 = g1_key3 = 152\n" // здесь ключом является только g1_key2
    //пустая строка после описания группы НЕ завершает группу
    "\n"
    "key2 = value2\n"
    "key4 = \"long\\ \n"
    "  string\"\n"
    "\n"
    "; многострочный коммент\n"
    "; ДО переменной inner_key2\n"
    "key2\\inner_key2 = a /*многострочный коммент\nпосле переменной inner_key2*/\n"
    "key2/inner_key2 = b\n" // "/" и "\" равнозначны при обработке
    "key3\\inner_key3 = a\n"
    "key3/inner_key33 = b\n"
    "[group 2]\n"
    "g2_string = one line string\n"
    "g2 string2 = big\n"
    "            line string\n" //пробелы после переноса строки должны стать равны одному пробелу
                                //при записи в файл, дополнить пробелами до знака "равно"
    //комментарий применится только для конечного элемента
    "; коммент ДО переменной inner_inner_key\n"
    "g2_key\\inner_key\\inner_inner_key = inner_inner_value ; вложенные значения и группы значений\n"
    "\n"
    "[group_3]\n"
    "g3_key = 15\n"
    "g3_key = \n"
    "g3_key = true\n"
    );


TEST(INI, main_parser) {
    using namespace simpleapi;

    Config json;
    CommentDesign cd;
    cd.with_comments = true;
    cd.oneline_comment_variants.push_back({';', 0});

    json.parseIni(ini_example_string, cd);
    EXPECT_EQ(json.size(), 6);

    ASSERT_TRUE(json.containsKey("key"));
    {
        ASSERT_TRUE(json["key"].isNumber());
        EXPECT_EQ(json["key"], 15);
        EXPECT_EQ(json.get_comment("key").prefix(), "комментарий\nвторая строка комментария");
    }

    ASSERT_TRUE(json.containsKey("array"));
    {
        ASSERT_TRUE(json["array"].isArray());
        Config ja = json["array"];
        {
            EXPECT_EQ(ja.size(), 3);
            EXPECT_TRUE(ja[0].isString());
            EXPECT_EQ(ja[0], "a");

            EXPECT_TRUE(ja[1].isNumber());
            EXPECT_EQ(ja[1], 15);

            EXPECT_TRUE(ja[2].isBool());
            EXPECT_EQ(ja[2], true);
        }
    }

    ASSERT_TRUE(json.containsKey("json"));
    {
        ASSERT_TRUE(json["json"].isJson());
        Config j = json["json"];
        {
            EXPECT_EQ(j.size(), 1);

            ASSERT_TRUE(j.containsKey("inner_array"));
            {
                ASSERT_TRUE(j["inner_array"].isArray());
                Config ja = j["inner_array"];
                {
                    EXPECT_EQ(ja.size(), 3);

                    EXPECT_TRUE(ja[0].isString());
                    EXPECT_EQ(ja[0], "a");

                    EXPECT_TRUE(ja[1].isNumber());
                    EXPECT_EQ(ja[1], 4.0);

                    EXPECT_TRUE(ja[2].isString());
                    EXPECT_EQ(ja[2], "asd string");
                }
            }
        }
    }

    //группы значений
    ASSERT_TRUE(json.containsKey("group 1")); {
        ASSERT_TRUE(json["group 1"].isJson());
        Config j = json["group 1"];
        {
            EXPECT_EQ(j.size(), 5);

            ASSERT_TRUE(j.containsKey("g1_key"));
            ASSERT_TRUE(j["g1_key"].isString());
            EXPECT_EQ(j["g1_key"], "value");

            ASSERT_TRUE(j.containsKey("g1_key2"));
            ASSERT_TRUE(j["g1_key2"].isString());
            EXPECT_EQ(j["g1_key2"], "g1_key3 = 152");

            ASSERT_TRUE(j.containsKey("key2"));
            {
                ASSERT_TRUE(j["key2"].isArray());
                Config ja = j["key2"];
                {
                    ASSERT_EQ(ja.size(), 2);

                    ASSERT_TRUE(ja[0].isString());
                    EXPECT_EQ(ja[0], "value2");

                    ASSERT_TRUE(ja[1].isJson());
                    Config jj = ja[1];
                    {
                        ASSERT_EQ(jj.size(), 1);
                        ASSERT_TRUE(jj.containsKey("inner_key2"));
                        ASSERT_TRUE(jj["inner_key2"].isArray());
                        Config ja2 = jj["inner_key2"];
                        {
                            EXPECT_EQ(ja2.size(), 2);

                            ASSERT_TRUE(ja2[0].isString());
                            EXPECT_EQ(ja2[0], "a");
                            EXPECT_EQ(ja2[0].getComment().prefix(),
                                      "многострочный коммент\nДО переменной inner_key2");
                            EXPECT_EQ(ja2[0].getComment().suffix(),
                                      "многострочный коммент\nпосле переменной inner_key2");

                            ASSERT_TRUE(ja2[1].isString());
                            EXPECT_EQ(ja2[1], "b");
                        }
                    }
                }
            }

            ASSERT_TRUE(j.containsKey("key3"));
            {
                ASSERT_TRUE(j["key3"].isJson());
                Config ja = j["key3"];
                {
                    ASSERT_EQ(ja.size(), 2);

                    ASSERT_TRUE(ja.containsKey("inner_key3"));
                    ASSERT_TRUE(ja["inner_key3"].isString());
                    EXPECT_EQ(ja["inner_key3"], "a");

                    ASSERT_TRUE(ja.containsKey("inner_key33"));
                    ASSERT_TRUE(ja["inner_key33"].isString());
                    EXPECT_EQ(ja["inner_key33"], "b");
                }
            }

            ASSERT_TRUE(j.containsKey("key4"));
            {
                ASSERT_TRUE(j["key4"].isString());
                EXPECT_EQ(j["key4"], "long\nstring");
            }

            EXPECT_EQ(j.getComment().suffix(), "ещё комментарий");
        }
    }

    ASSERT_EQ(json.containsKey("group 2"), true); {
        ASSERT_TRUE(json["group 2"].isJson());
        Config j = json["group 2"];
        {
            ASSERT_EQ(j.size(), 3);

            ASSERT_TRUE(j.containsKey("g2_string"));
            ASSERT_TRUE(j["g2_string"].isString());
            EXPECT_EQ(j["g2_string"], "one line string");

            ASSERT_TRUE(j.containsKey("g2 string2"));
            ASSERT_TRUE(j["g2 string2"].isString());
            EXPECT_EQ(j["g2 string2"], "big\nline string");

            ASSERT_TRUE(j.containsKey("g2_key"));
            ASSERT_TRUE(j["g2_key"].isJson());
            Config j2 = j["g2_key"];
            {
                ASSERT_TRUE(j2["inner_key"].isJson());
                Config j3 = j2["inner_key"];
                {
                    EXPECT_EQ(j3.get_comment("inner_inner_key").prefix(), "коммент ДО переменной inner_inner_key");
                    EXPECT_EQ(j3.get_comment("inner_inner_key").suffix(), "вложенные значения и группы значений");
                }
            }
        }
    }

    ASSERT_EQ(json.containsKey("group_3"), true); {
        ASSERT_TRUE(json["group_3"].isJson());
        Config j = json["group_3"];
        {
            ASSERT_EQ(j.size(), 1);
            ASSERT_EQ(j.containsKey("g3_key"), true);
            ASSERT_TRUE(j["g3_key"].isArray());
            Config ja = j["g3_key"];
            {
                ASSERT_EQ(ja.size(), 3);

                EXPECT_TRUE(ja[0].isNumber());
                ASSERT_EQ(ja[0], 15);

                ASSERT_TRUE(ja[1].isNull());

                ASSERT_TRUE(ja[2].isBool());
                EXPECT_EQ(ja[2], true);
            }
        }
    }

}

//TODO: TEST(INI, main_writer_map) {}
//TODO: TEST(INI, main_writer_array) {}

TEST(INI, writer_large_strings) {
    using namespace simpleapi;
    using namespace tools;

    std::string json_string_example2 =
        "/*asd*/\n"
        "a = \n\"very\n"
        "large\n"
        "string\""
        ;
    CommentDesign cd;
    cd.with_comments = true;
    Config cfg = ParseJson(json_string_example2, cd);

    std::string expected =
        "// asd\n"
        "a = \"very \\\n"
        "    large \\\n"
        "    string\"\n"
        ""
        ;
    std::string res = cfg.toString(simpleapi::ConfigFormat::eINI, cd);
    EXPECT_EQ(res, expected);
}

TEST(INI, parse_with_json_value) {
    using namespace simpleapi;
    using namespace tools;

    std::string input =
        "[hw_array]\n"
        "gcom = { hw_id=136 \\\n"
        " radio_id=[1, 2] \\\n"
        " radio_id=3 }\n"
        "\n"
        ;

    Config cfg = ParseIni(input);
    ASSERT_TRUE(cfg.containsKey("hw_array"));
    Config cfg_hw_array = cfg["hw_array"];
    ASSERT_TRUE(cfg_hw_array.containsKey("gcom"));
    Config cfg_gcom = cfg_hw_array["gcom"];
    EXPECT_TRUE(cfg_gcom.isJson());
}

TEST(INI, custom_test_for_writer) {
    using namespace simpleapi;
    Config example_config(ValueType::eJson);
    example_config["test1"]    = "test string";
    example_config["test2"]    = 157;
    example_config["test_arr"] = Config(ValueType::eArray, "test string for array", 1123);
    CommentDesign cd;
//    cd.with_comments = true;
//    cd.oneline_comment_variants.push_back({';', 0});
//    example_config["test"].setPrefixComment("пример использования SimpleAPI для записи массива в формате INI");

    std::string result_string = example_config.toString(simpleapi::ConfigFormat::eINI, cd);

    std::string example_string =
        "test1 = \"test string\"\n"
        "test2 = 157\n"
        "test_arr = [\"test string for array\", 1123]\n"
        ;
    EXPECT_EQ(example_string, result_string);
}

TEST(INI, correct_comments_writer) {
    using namespace simpleapi;
    Config example_config(ValueType::eJson);
//    example_config["test1"]     = "test string";
//    example_config["test2"]     = 157;
    example_config["test_arr1"] = Config(ValueType::eArray, "test string for array", 1123);
    example_config["test_arr2"] = Config(ValueType::eArray);
    example_config["test_arr2"].push_back(Config(ValueType::eJson, "innerJson_el", "some value"));
//    example_config["test_arr2"].push_back("simple string");
//    example_config["test_arr2"].push_back(1412.0f);
    CommentDesign cd;
    cd.with_comments = true;
    cd.oneline_comment_variants.push_back({';', 0});
    example_config["test_arr1"].setSuffixComment("test suffix comment");
    example_config["test_arr2"].setPrefixComment("test prefix comment");

    std::string result_string = example_config.toString(simpleapi::ConfigFormat::eINI, cd);

    std::string example_string =
//        "test1 = \"test string\"\n"
//        "test2 = 157\n"
        "test_arr1 = [\"test string for array\", 1123] ; test suffix comment\n"
        "; test prefix comment\n"
        "test_arr2/innerJson_el = \"some value\"\n"
//        "test_arr2 = \"simple string\"\n"
//        "test_arr2 = 1412\n"
        ;

    EXPECT_EQ(example_string, result_string);
}

TEST(INI, correct_comments_writer2) {
    using namespace simpleapi;
    Config example_config(ValueType::eJson);
    example_config["test1"]     = "test string";
    example_config["test2"]     = 157;
    CommentDesign cd;
    cd.with_comments = true;
    cd.oneline_comment_variants.push_back({';', 0});
    example_config["test1"].setSuffixComment("test suffix comment");
    example_config["test2"].setPrefixComment("test prefix comment");

    std::string result_string = example_config.toString(simpleapi::ConfigFormat::eINI, cd);

    std::string example_string =
        "test1 = \"test string\" ; test suffix comment\n"
        "; test prefix comment\n"
        "test2 = 157\n"
        ;

    EXPECT_EQ(example_string, result_string);
}

TEST(INI, write_infinities) {
    using namespace simpleapi;
    Config cfg;
    cfg["a"] = std::numeric_limits<long double>::infinity();
    cfg["b"] = -std::numeric_limits<long double>::infinity();
    std::string test = cfg.toString(ConfigFormat::eINI);

    EXPECT_EQ(test, "a = inf\n"
                    "b = -inf\n");
}

TEST(INI, parse_infinities) {
    using namespace simpleapi;
    Config cfg;
    cfg.parseIni("a = inf\n"
                 "b = -inf\n"
                 "c = infinity\n"
                 "d = -infinity\n"
                 );

    EXPECT_TRUE(cfg["a"].isString());
    EXPECT_TRUE(cfg["b"].isString());
    EXPECT_TRUE(cfg["c"].isString());
    EXPECT_TRUE(cfg["d"].isString());
}

TEST(INI, parse_MAC_address) {
    using namespace simpleapi;
    Config cfg;
    cfg.parseIni("MAC = \"11:22:33:44:55:66\"\n");
    EXPECT_EQ(cfg.size(), 1);

    cfg.parseIni("MAC = 11:22:33:44:55:66\n");
    EXPECT_EQ(cfg.size(), 1);
}

TEST(INI, writer_groups) {
    using namespace simpleapi;

    Config cfg;
    cfg[""]["a"] = 1;
    cfg[""]["b"] = 2;
    cfg["group 2"]["a"] = 3;
    cfg["group 2"]["b"] = 4;

    const std::string expected_result =
        "a = 1\n"
        "b = 2\n"
        "\n"
        "[group 2]\n"
        "a = 3\n"
        "b = 4\n";

    EXPECT_EQ(cfg.toString(simpleapi::ConfigFormat::eINI), expected_result);
}

TEST(INI, writer_different_len_of_keys_alignment) {
    using namespace simpleapi;
    Config cfg;
    cfg["a"]   = 1;
    cfg["aa"]  = 2;
    cfg["aaa"] = 3;

    std::string expected_result =
        "a   = 1\n"
        "aa  = 2\n"
        "aaa = 3\n";
    EXPECT_EQ(cfg.toString(simpleapi::ConfigFormat::eINI), expected_result);

    cfg.writerStile().max_key_length = 2; // ограничит максимальное выравнивание ключей
    expected_result =
        "a  = 1\n"
        "aa = 2\n"
        "aaa = 3\n";
    EXPECT_EQ(cfg.toString(simpleapi::ConfigFormat::eINI), expected_result);
}
