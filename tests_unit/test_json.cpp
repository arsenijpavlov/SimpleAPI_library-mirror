#include <SimpleAPI.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <regex>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}


//========================================================================================
using namespace simpleapi;
using namespace tools;

Config json_example(ValueType::eJson,
                    "key_0", "first",
                    "key_1", Config(2),
                    "key_2", Config(3.1),
                    "key_3", Config(true)
                    );
Config json2_example(
    std::vector<std::pair<std::string, Config>>{
        std::make_pair("key_0", Config("first")),
        std::make_pair("key_1", Config(2)),
        std::make_pair("key_2", Config(3.1)),
        std::make_pair("key_3", Config(true))
    }
);

std::string json_string_example = std::string(
    "{"
    //пробелы и табуляции
    "   \t"
    //однострочные комментарии (используются знаки по умолчанию)
    "//\n"
    //многострочные комментарии (используются знаки по умолчанию)
    "/*\n*/"
    //поля
    "\"number\":182,\n"
    //иной вариант разделителя '='
    "\"bool\"=true,\n"
    "\"null\"=NuLL,\n"
    "\"null2\"=,\n"
    //перенос строки равнозначен разделителю ','
    "\"string\":\"string_value\"\n"
    "\"json\" : {"
    "   \"string\":\"inner_string_value\",\n"
    "   \"string2\" : 150"
    "}"
    ",\n"
    //перенос строки равнозначен разделителю ',' (массивы)
    "\"array\":[\"string_value\"\n true]\n"
    "}"
    );

std::string inner_json_example = "{\"string\":\"inner_string_value\",\n \"string2\" : 150 }";
std::string inner_array_example = "[\"string_value\"\n true]";


TEST(JSON, copy) {
    Config j1(std::vector<std::pair<std::string, Config>>{
               std::make_pair("asd", Config("123"))
    });
    j1.push_at("asa", "asdd");
    Config j2 = j1;

    EXPECT_EQ(j1.size(), j2.size());
}

TEST(JSON, append_json) {
    Config j1(ValueType::eJson, "asd", "asd");
    uint8_t pre_size = j1.size();

    Config j2(ValueType::eJson);
    j2.push_at("num1", 15);
    j2.push_at("num2", 1.5);
    j2.push_at("bool", true);

    j1.append(j2);

    EXPECT_EQ(pre_size + j2.size(), j1.size());
}

TEST(JSON, parse) {
    std::string string_json = json_string_example;
    std::string string_json2 = json_string_example;
    Config json;

    Config inner_json;
    inner_json.parseJson(inner_json_example);
    ASSERT_EQ(inner_json.size(), 2);
    EXPECT_TRUE(inner_json["string"].isString());
    EXPECT_TRUE(inner_json["string2"].isNumber());

    Config inner_array;
    inner_array.parseJson(inner_array_example);
    ASSERT_EQ(inner_array.size(), 2);
    EXPECT_TRUE(inner_array[0].isString());
    EXPECT_TRUE(inner_array[1].isBool());

    json.parseJson(string_json);
    EXPECT_EQ(json.size(), 7);

    EXPECT_TRUE(json["number"].isNumber());
    EXPECT_EQ(json["number"].getNumber(),           182);
    EXPECT_EQ(json["number"],                       182);

    EXPECT_TRUE(json["bool"].isBool());
    EXPECT_EQ(json["bool"].getBool(),               true);
    EXPECT_EQ(json["bool"].getBool(),               true);

    EXPECT_TRUE(json["null"].isNull());
    EXPECT_EQ(json["null"],                         Config());
    EXPECT_EQ(json["null"],                         Config(ValueType::eNull));

    EXPECT_TRUE(json["null2"].isNull());
    EXPECT_EQ(json["null2"],                        Config());
    EXPECT_EQ(json["null2"],                        Config(ValueType::eNull));

    EXPECT_TRUE(json["string"].isString());
    EXPECT_EQ(json["string"].getString(),           "string_value");
    EXPECT_EQ(json["string"],                       "string_value");

    EXPECT_FALSE(json["json"].isNull());
    EXPECT_TRUE(json["json"].isJson());
    ASSERT_EQ(json["json"].size(),                  2);
    EXPECT_EQ(json["json"]["string"].getString(),   "inner_string_value");

    EXPECT_TRUE(json["array"].isArray());
    ASSERT_EQ(json["array"].size(),                 2);
    EXPECT_EQ(json["array"][0].getString(),         "string_value");
    EXPECT_EQ(json["array"][1].getBool(),           true);

    //повторная обработка (очистка, новое заполнение)
    json.parseJson(string_json2);
    EXPECT_EQ(json.size(), 7);

    CommentDesign cd;
    json.parse(std::move(string_json), ConfigFormat::eJSON, cd);
    ASSERT_EQ(json.size(), 7);
}

TEST(JSON, parse2) {
    std::string string_json = R"({"get":["chip_key"]})";
    Config json;
    json.parseJson(string_json);

    EXPECT_EQ(json.size(), 1);
}

TEST(JSON, parse3) {
    std::string string_json = R"({"Hello":"WORLD!"})";
    Config json;
    json.parseJson(string_json);

    EXPECT_EQ(json.size(), 1);
}

TEST(JSON, parse_custom_string_elements) {
    //ВСЕ экранированные символы должны попасть в значение без изменений
    std::string temp_string = "\"asd\\\"\\b\\f\\n\\r\\tdsa\"";
    std::string json_string = std::string("{ ")
                              + "string:" + temp_string
                              + ", array:[" + temp_string + "]"
                              + "}";

    //исходная строка без кавычек по бокам
    std::string temp_string_without_quotes = temp_string.substr(1, temp_string.length() - 2);

    Config inner_array;
    std::string inner_array_str = "[" + temp_string + "]";
    inner_array.parse(inner_array_str, ConfigFormat::eJSON);
    ASSERT_EQ(inner_array.size(),           1);

    Config json;
    json.parseJson(json_string);

    ASSERT_TRUE(json.isJson());
    EXPECT_EQ(json.size(),                  2);

    EXPECT_TRUE(json["string"].isString());
    EXPECT_EQ(json["string"],               temp_string_without_quotes);
    EXPECT_EQ(json["string"].toString(),    temp_string_without_quotes);

    ASSERT_TRUE(json["array"].isArray());
    ASSERT_EQ(json["array"].size(),         1);
    EXPECT_EQ(json["array"][0],             temp_string_without_quotes);
    EXPECT_EQ(json["array"][0].toString(),  temp_string_without_quotes);
}

TEST(JSON, parse_error) {
    std::string string_json;
    Config json;

    //некорректное значение числа, пробелов нет ==> это строка!
    string_json = "{key:15.4.3}";
    json.parseJson(string_json);
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json[0].isString());
    EXPECT_EQ(json["key"], "15.4.3");

    //некорректное значение числа, пробелов нет ==> это строка!
    string_json = "{key:15e43}";
    try {
        json.parseJson(string_json);
    } catch(...) {}
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json[0].isNumber());

    //пробелы в значении bool
    string_json = "{key:tru e}";
//    EXPECT_ANY_THROW(
    try{
        json.parseJson(string_json);
    } catch(...){
        std::cout << std::endl;
    };
//        );
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json["key"].isString()) << json["key"].toString();

    //некорректное значение bool ==> это строка!
    string_json = "{key:truee}";
    json.parseJson(string_json);
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json[0].isString());
}

TEST(JSON, writeFile_custom_tabulation_level) {
    Config json = json_example;

    bool ret = json.writeFile("../tests/test_writer_tabs1.json", ConfigFormat::eJSON, {}, 1);
    EXPECT_TRUE(ret);

    ret = json.writeFile("../tests/test_writer_tabs3.json", ConfigFormat::eJSON, {}, 3);
    EXPECT_TRUE(ret);
}

TEST(JSON, write_file) {
    Config json = json_example;

    bool ret = json.writeFile("../tests/test_writer_with_spaces.json", ConfigFormat::eJSON);
    EXPECT_TRUE(ret);

    ret = json.writeFile("../tests/test_writer.json", ConfigFormat::eONLY_VALUE);
    EXPECT_TRUE(ret);
}

TEST(JSON, parse_with_comments) {
    CommentDesign cd;
    cd.with_comments = true;

    std::string test_input = "{a:15,/*s_comment*//*p1_comment*//*p2_comment*/b:120}";
    Config json;
    json.parseJson(test_input, cd);

    EXPECT_EQ(json.get_suffix_comment("a"), "s_comment");
    EXPECT_EQ(json.get_prefix_comment("b"), "p1_comment\n"
                                            "p2_comment");

    test_input = "{a:15, //s1_comment\n"
                 "//p2_comment\n"
                 "b:20 //s2_comment\n"
                 "} //sf_comment\n";
    json.parseJson(test_input, cd);

    EXPECT_EQ(json.get_suffix_comment("a"), "s1_comment");
    EXPECT_EQ(json.get_prefix_comment("b"), "p2_comment");
    EXPECT_EQ(json.get_suffix_comment("b"), "s2_comment");
    EXPECT_EQ(json.getSuffixComment(), "sf_comment");
}

TEST(JSON, parse_with_comments2) {
    CommentDesign cd;
    cd.with_comments = true;

    std::string test_input = "{\n"
                             "a:15,\n"
                             "/*p1_comment*/\n"
                             "b:120\n"
                             "}";
    Config json;
    json.parseJson(test_input, cd);

    EXPECT_EQ(json.get_suffix_comment("a"), "");
    EXPECT_EQ(json.get_prefix_comment("b"), "p1_comment");

    test_input = "{\n"
                 "a:15,\n"
                 "//p2_comment\n"
                 "b:20\n"
                 "}\n";
    json.parseJson(test_input, cd);

    EXPECT_EQ(json.get_suffix_comment("a"), "");
    EXPECT_EQ(json.get_prefix_comment("b"), "p2_comment");
}

TEST(JSON, write_and_read_file_comment) {
    Config json(ValueType::eJson);
    json.parseJson(json_string_example);

    CommentDesign& cd = json.getCommentDesign();
    cd.opt_multiline_column_size = 20;
    cd.opt_multiline_border = '#';
    cd.with_comments = true;
    cd.opt_multiline_border_at_content_line = true;

    std::string main_comment = "1;losdihfg2;slopighsd3;pogihvd4;pfgvibhdfns5;ipnbedf6 7;voihnaern8 som9 word1...";
    json.setPrefixComment(main_comment);
    json.setSuffixComment(main_comment);
    json.set_prefix_comment("bool", "bool some\nwords...");
    json.set_suffix_comment("bool", "bool some\nwords...");
    json.set_prefix_comment("string", "<string> prefix comment");
    json.set_prefix_comment("array", "<array> prefix comment");
    json.set_suffix_comment("array", "<array> suffix comment");
    json.set_prefix_comment("json", "<json> prefix comment");
    json.set_suffix_comment("json", "<json> suffix comment");

    json["json"].set_prefix_comment(0, "json first element\ncomment");
    json["array"].set_prefix_comment(1, "array second element\n comment_");

    std::string path = "../tests/test_writer_with_comments.json";
    json.writeFile(path, ConfigFormat::eJSON, cd, 0);

    Config json2;
    try {
        json2.readFileJson(path, cd);
    } catch (const std::exception& e) {
        FAIL() << e.what();
    }

    EXPECT_EQ(json2.size(), json.size());
    EXPECT_EQ(json2.getCommentDesign(), json.getCommentDesign());

    //при сравнении Comment как цельной единицы, prefix и suffix будут проверены нормализовано
    EXPECT_EQ(json2.getComment(), json.getComment());
    EXPECT_EQ(utils::GetNormalizeString(json2.getPrefixComment()), utils::GetNormalizeString(json.getPrefixComment()));
    EXPECT_EQ(utils::GetNormalizeString(json2.getSuffixComment()), utils::GetNormalizeString(json.getSuffixComment()));
    EXPECT_EQ(utils::GetNormalizeString(json2.getPrefixComment()), utils::GetNormalizeString(main_comment));
    EXPECT_EQ(utils::GetNormalizeString(json2.getSuffixComment()), utils::GetNormalizeString(main_comment));

    // проверка комментариев должна учитывать переносы строк (преобразование комментариев при чтении/записи)
    auto FromTo = [&json](const std::string& s) -> std::string {
        return FromComment(ToComment(s, json.getCommentDesign()), json.getCommentDesign());
    };
    auto FromTo_Suffix = [&json](const std::string& s) -> std::string {
        CommentDesign temp_cd = json.getCommentDesign();
        temp_cd.opt_multiline_column_size = 0;
        return FromComment(ToComment(s, temp_cd), temp_cd);
    };

    EXPECT_EQ(json2.get_comment("bool"), json.get_comment("bool"));
    EXPECT_EQ(json2.get_prefix_comment("bool"), FromTo(json.get_prefix_comment("bool")));
    EXPECT_EQ(json2.get_suffix_comment("bool"), FromTo_Suffix(json.get_suffix_comment("bool")));

    EXPECT_EQ(json2.get_comment("string"), json.get_comment("string"));
    EXPECT_EQ(json2.get_prefix_comment("string"), FromTo(json.get_prefix_comment("string")));
    EXPECT_EQ(json2.get_suffix_comment("string"), FromTo_Suffix(json.get_suffix_comment("string")));

    //т.к. CommentDesign передаётся рекурсивно с верхних уровней, при чтении повлияет на все подуровни
    //нюанс актуален для контейнеров
    Comment c1 = json.get_comment("array");
    c1.setDesign(json.getCommentDesign());
    Comment c2 = json2.get_comment("array");
    EXPECT_EQ(c1, c2);
    EXPECT_EQ(c1.commentDesign(), c2.commentDesign());
    EXPECT_EQ(json2.get_prefix_comment("array"), FromTo(json.get_prefix_comment("array")));
    EXPECT_EQ(json2.get_suffix_comment("array"), FromTo_Suffix(json.get_suffix_comment("array")));

    EXPECT_EQ(json2["json"].get_comment(0), json["json"].get_comment(0));
    EXPECT_EQ(json2["json"].get_prefix_comment(0), FromTo_Suffix(json["json"].get_prefix_comment(0)));
    EXPECT_EQ(json2["json"].get_suffix_comment(0), FromTo_Suffix(json["json"].get_suffix_comment(0)));

    EXPECT_EQ(json2["array"].get_comment(0), json["array"].get_comment(0));
    EXPECT_EQ(json2["array"].get_prefix_comment(0), FromTo_Suffix(json["array"].get_prefix_comment(0)));
    EXPECT_EQ(json2["array"].get_suffix_comment(0), FromTo_Suffix(json["array"].get_suffix_comment(0)));
}

TEST(JSON, read_file) {
    std::string path = "../tests/test_reader.json";

    std::ofstream file_example_creator(path);
    if(!file_example_creator.is_open())
        FAIL();
    file_example_creator.write(json_string_example.c_str(), json_string_example.size());
    file_example_creator.close();

    Config json;
    json.readFile(path, ConfigFormat::eJSON);

    Config json2;
    json2.parseJson(json_string_example);
    EXPECT_EQ(json.size(), json2.size());
}

TEST(JSON, read_file_comment) {
    Config json;
    json.parseJson(json_string_example);
    json.getCommentDesign().opt_multiline_column_size = 20;
    json.getCommentDesign().with_comments = true;

    std::string preview_comment = "1;losdihfg2;slopighsd3;pogihvd4;pfgvibhdfns5;ipnbedf6 7;voihnaern8 som9 word1...";
    json.setPrefixComment(preview_comment);
    std::string comment1 = "some words...";
    std::string comment2 = "some many words1...";
    std::string comment3 = "some many words2...";
    std::string comment4 = "json element comment";
    std::string comment5 = "array element comment_";
    json.set_comment("bool", comment1);
    json.set_comment("string", comment2);
    json.set_suffix_comment("array", comment3);
    json["json"].set_prefix_comment(0, comment4);
    json["json"].set_prefix_comment(1, comment4);
    json["array"].set_comment(0,  comment5);

    //для проверки
    preview_comment = FromComment(ToComment(preview_comment, json.getCommentDesign()),
                                  json.getCommentDesign());
    comment5 = FromComment(ToComment(comment5, json.getCommentDesign()),
                           json.getCommentDesign());

    std::string path = "../tests/test_writer_with_comments.json";
    std::ofstream file(path);
    if (!file.is_open())
        FAIL();
    file << json.toString(ConfigFormat::eJSON, {}, 0) << std::endl;
    file.close();
    //========================================================================

    CommentDesign cd;
    cd.with_comments = true;

    Config json2;
    json2.readFile(path, ConfigFormat::eJSON, cd); //по умолчанию считывается JSON формат
    ASSERT_EQ(false, json2.isEmpty());

    EXPECT_EQ(json2.getPrefixComment(), preview_comment);
    ASSERT_EQ(json2.containsKey("bool"), true);
    EXPECT_EQ(json2.get_comment("bool").prefix(), comment1);
    ASSERT_EQ(json2.containsKey("string"), true);
    EXPECT_EQ(json2.get_comment("string").prefix(), comment2);
    ASSERT_EQ(json2.containsKey("array"), true);
    EXPECT_EQ(json2.get_comment("array").suffix(), comment3);
    ASSERT_EQ(json2.containsKey("json"), true);
    ASSERT_EQ(json2["json"].size() > 0, true);
    EXPECT_EQ(json2["json"].get_comment(0).prefix(), comment4);
    EXPECT_EQ(json2["json"].get_comment(1).prefix(), comment4);
    ASSERT_EQ(json2.containsKey("array"), true);
    EXPECT_EQ(json2["array"].get_comment(0).prefix(), comment5);
}

TEST(JSON, read_file_error) {
    //файла не существует
    Config json;
    json.readFile("json_file_not_found.json", ConfigFormat::eJSON);

    EXPECT_EQ(json.size(), 0);
}

TEST(JSON, put_and_get_elements) {
    //без const значения перместятся в Json и их нельзя будет проверить через EXPECT_EQ
    const std::string test_str    = "abc";
    const double test_num         = 15;
    const bool test_bool          = true;

    Config json; //пустой == null
    json.push_back("string", test_str); //json преобразован в тип Json
    json.push_back("number", test_num);
    json.push_back("bool", test_bool);
    EXPECT_EQ(json.size(), 3);

    EXPECT_EQ(test_str, json["string"].getString());
    EXPECT_EQ(test_num, json["number"].getNumber());
    EXPECT_EQ(test_bool, json["bool"].getBool());

    //предполагается, что пользователю не придётся использовать этот класс
    tools::ElementArray array;
    array.push_back("str");
    array.push_back(15);

    Config json2;
    json2.push_back("json", json);
    json2.push_back("array", array);

    EXPECT_EQ(json, json2["json"]);
    EXPECT_EQ(json["string"], json2["json"]["string"]);
    EXPECT_EQ(json["string"].getString(), json2["json"]["string"].getString());

    EXPECT_EQ(array, json2["array"]);

    EXPECT_EQ(array.size(), json2["array"].size());
    EXPECT_EQ(array[0], json2["array"][0]);
    EXPECT_EQ(array[0].getString(), json2["array"][0].getString());
}

TEST(JSON, update_value) {
    Config json(ValueType::eJson, "key", 15);

    json.push_back("key", 20);
    EXPECT_EQ(json["key"].getNumber(), 20);

    json.push_back("key", 30);
    EXPECT_EQ(json["key"].getNumber(), 30);

    json.push_back("key", 40);
    EXPECT_EQ(json["key"].getNumber(), 40);

    json.push_back("key", true);
    EXPECT_EQ(json["key"].getBool(), true);
}

TEST(JSON, get_index) {
    Config json = json_example;

    EXPECT_EQ(json[0].getString(), "first");
    EXPECT_EQ(json[1].getNumber(), 2);
    EXPECT_EQ(json[2].getNumber(), 3.1);
    EXPECT_EQ(json[3].getBool(), true);
}

TEST(JSON, get_index_error) {
    Config json = json_example;

    try {
        json[5];
    } catch (std::exception& e) {
        return SUCCEED();
    }

    FAIL();
}

TEST(JSON, get_complex_name) {
    Config j;
    j.push_back("json_num", 15);
    Config a;
    a.push_back(j);

    Config j_main;
    j_main.push_at("array", a);

    double d = j_main[{"array", 0, "json_num"}].getNumber();
    EXPECT_EQ(15, d);
}

TEST(JSON, get_key) {
    Config json = json_example;

    EXPECT_EQ(json.get_at("key_0").getString(), "first");
    EXPECT_EQ(json.get_at("key_1").getNumber(), 2);
    EXPECT_EQ(json.get_at("key_2").getNumber(), 3.1);
    EXPECT_EQ(json.get_at("key_3").getBool(), true);
}

TEST(JSON, insert) {
    Config json = json_example;

    json.insert_at(2, "inner_json1", json_example);
    EXPECT_EQ(json[2].getType(), ValueType::eJson);

    json.insert_at(4, "inner_json2", json_example);
    EXPECT_EQ(json[4].getType(), ValueType::eJson);
}

TEST(JSON, insert_before) {
    Config json = json_example;
    json.insert_before("key_1", "inner_json", json_example);

    EXPECT_EQ(json[1].getType(), ValueType::eJson);
}

TEST(JSON, insert_after) {
    Config json = json_example;
    json.insert_after("key_1", "inner_json", json_example);

    EXPECT_EQ(json[2].getType(), ValueType::eJson);
}

TEST(JSON, erase_at) {
    Config json = json_example;
    json.erase_at(2);
    EXPECT_TRUE(json[2].isBool());
}

TEST(JSON, erase_key) {
    Config json = json_example;

    json.erase_at("key_0");
    EXPECT_TRUE(json[0].isNumber());
}

TEST(JSON, check_numbers) {
    std::map<std::string, bool> map_numbers;
    map_numbers.insert(std::make_pair("1",          true));
    map_numbers.insert(std::make_pair("1.1",        true));
    map_numbers.insert(std::make_pair("1e1",        true));
    map_numbers.insert(std::make_pair("1.1e1",      true));
    map_numbers.insert(std::make_pair("1.1.1",      false));
    map_numbers.insert(std::make_pair("e1",         false));
    map_numbers.insert(std::make_pair("f1",         false));
    map_numbers.insert(std::make_pair(".1",         true));
    map_numbers.insert(std::make_pair("a1e2",       false));
    map_numbers.insert(std::make_pair("145o",       false));
    map_numbers.insert(std::make_pair("1eu2",       false));
    map_numbers.insert(std::make_pair("1E5",        true));
    map_numbers.insert(std::make_pair("1f",         true));
    map_numbers.insert(std::make_pair("1e",         true));
    map_numbers.insert(std::make_pair("1ef",        true));
    map_numbers.insert(std::make_pair("1e1.1",      false));
    map_numbers.insert(std::make_pair("1.1e-1",     true));
    map_numbers.insert(std::make_pair("-1.1e-1",    true));
    map_numbers.insert(std::make_pair("-1.1e+1",    true));
    map_numbers.insert(std::make_pair("-1.1e+1e",   false));
    map_numbers.insert(std::make_pair("f",          false));

    for(auto it = map_numbers.cbegin(); it != map_numbers.cend(); it++) {
        Config json;
        json.parseJson("{number:" + it->first + "}");
        EXPECT_TRUE(json.isJson());
        EXPECT_EQ(json.size(), 1);
        EXPECT_EQ(json[0].isNumber(), it->second);
    }
}

TEST(JSON, parse_simple_element) {
    std::string test_file_string = "k = 1";
    Config json;
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = \"string line\"";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = true";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = ";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = null";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = word";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = {a=b}";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "k = [a,b]";
    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);
}

TEST(JSON, parse_simple_element_with_comments) {
    CommentDesign cd;
    cd.with_comments = true;

    std::string test_file_string = "//b\n"
                                   "k=1"
                                   "//a";
    Config json;

    json.parseJson(test_file_string, cd);
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json.getComment().prefix().empty());
    EXPECT_FALSE(json[0].getComment().prefix().empty());
    EXPECT_EQ(json[0].getComment().prefix(), "b");
    EXPECT_FALSE(json[0].getComment().suffix().empty());
    EXPECT_EQ(json[0].getComment().suffix(), "a");

    json.parseJson(test_file_string);
    EXPECT_EQ(json.size(), 1);

    test_file_string = "//b\n"
                       "k=1";
    json.parseJson(test_file_string, cd);
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json.getComment().prefix().empty());
    EXPECT_FALSE(json[0].getComment().prefix().empty());
    EXPECT_TRUE(json[0].getComment().suffix().empty());

    test_file_string = "\n"
                       "k=1"
                       "//a";
    json.parseJson(test_file_string, cd);
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json.getComment().prefix().empty());
    EXPECT_TRUE(json[0].getComment().prefix().empty());
    EXPECT_FALSE(json[0].getComment().suffix().empty());
}

TEST(JSON, parse_element_with_many_prefix_comments) {
    CommentDesign cd;
    cd.with_comments = true;

    std::string test_file_string = "/*b1*/\n"
                                   "/*b2*/\n"
                                   "k=1"
                                   "//a";
    Config json;

    json.parseJson(test_file_string, cd);
    EXPECT_EQ(json.size(), 1);
    EXPECT_TRUE(json.getComment().prefix().empty());
    EXPECT_FALSE(json[0].getComment().prefix().empty());
    EXPECT_EQ(json[0].getComment().prefix(), "b1\nb2");
    EXPECT_FALSE(json[0].getComment().suffix().empty());
    EXPECT_EQ(json[0].getComment().suffix(), "a");
}

TEST(JSON, get_deep_inner_element_1) {
    Config json_main(ValueType::eArray);
    Config json_inner_1(ValueType::eArray);
    Config json_inner_2(ValueType::eArray);
    Config json_inner_3(ValueType::eArray);

    json_inner_3.push_back(15);
    json_inner_2.push_back(json_inner_3);
    json_inner_1.push_back(json_inner_2);
    json_main.push_back(json_inner_1);

    Config json_result = json_main[{0, 0, 0, 0}];
    EXPECT_EQ(json_result.getNumber(), 15);

    json_result = json_main.get_at({0, 0, 0, 0});
    EXPECT_EQ(json_result.getNumber(), 15);
}

TEST(JSON, get_deep_inner_element_2) {
    Config json_main(ValueType::eJson);
    Config json_inner_1(ValueType::eJson);
    Config json_inner_2(ValueType::eJson);
    Config json_inner_3(ValueType::eJson);

    json_inner_3.push_back("key", 15);
    json_inner_2.push_back("key", json_inner_3);
    json_inner_1.push_back("key", json_inner_2);
    json_main.push_back("key", json_inner_1);

    Config json_result = json_main[{0,0,0, "key"}];
    EXPECT_EQ(json_result.getNumber(), 15);
}

TEST(JSON, contains) {
    Config json(ValueType::eJson, "k1", "v1");

    EXPECT_TRUE(json.containsKey("k1"));
    EXPECT_FALSE(json.containsKey("k2"));
    EXPECT_TRUE(json.containsValue("v1"));
    EXPECT_FALSE(json.containsValue("v2"));

    Config json2(ValueType::eJson);
    Config json3(ValueType::eJson);
    json.push_back("k2", json2);
    EXPECT_TRUE(json.containsValue(json2));
    EXPECT_TRUE(json.containsValue(json3));
    json3.push_back("kk1", "vv1");
    EXPECT_FALSE(json.containsValue(json3));
}


bool RegexCheckCounter(const std::string& error_str, const size_t& line_number,
                       const size_t& symbol_number)
{
    std::regex reg(R"(\[([0-9]+)\]\[([0-9]+)\])"); //ожидаем два совпадения по одной маске
    std::smatch matches;
    if(std::regex_search(error_str, matches, reg) && matches.size() == 3)
    {
        // std::cout << "matches size: " << matches.size() << std::endl;
        // std::cout << "matches: " << matches[1] << std::endl;
        // std::cout << "matches: " << matches[2] << std::endl;

        try {
            size_t x_coord = stoull(matches[1]);
            size_t y_coord = stoull(matches[2]);
            return x_coord == line_number && y_coord == symbol_number;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return false;
}

TEST(JSON, parser_symbols_counter) {
    Config json;
    bool regex_found;

    json.parseJson("{a=b #");
    EXPECT_TRUE(json.error()) << json.getError();
    regex_found = RegexCheckCounter(json.getError(), 0, 5);
    EXPECT_TRUE(regex_found);

    json.parseJson("{a=b,\n"
                   "c=d #");
    EXPECT_TRUE(json.error()) << json.getError();;
    regex_found = RegexCheckCounter(json.getError(), 1, 5);
    EXPECT_TRUE(regex_found);

    json.parseJson("# #");
    EXPECT_TRUE(json.error()) << json.getError();;
    regex_found = RegexCheckCounter(json.getError(), 0, 2);
    EXPECT_TRUE(regex_found);
}

TEST(JSON, parser_simple_value) {
    Config cfg = ParseJson("15");
    EXPECT_EQ(cfg, 15);

    cfg = ParseJson("string");
    EXPECT_EQ(cfg, "string");

    cfg = ParseJson("{string");
    EXPECT_NE(cfg, "{string"); //должно распарсить как Json, но при этом Json некорректный - 0 элементов
}

TEST(JSON, parse_json_with_empty_element) {
    Config cfg = ParseJson("{a=b,c=,d=e}");
    ASSERT_EQ(cfg.size(), 3);
    Config cfg2 = Config(ValueType::eJson, "a", "b", "c", nullptr, "d", "e");
    EXPECT_EQ(cfg, cfg2);
}

TEST(JSON, parse_incorrect) {
    Config cfg = ParseJson("{}]");
    EXPECT_TRUE(cfg.error());
}

TEST(JSON, parse_incorrect2) {
    Config cfg = ParseJson("{{}]}");
    EXPECT_TRUE(cfg.error());
}

TEST(JSON, parse_incorrect3) {
    Config cfg = ParseJson("{{a=b}]}");
    EXPECT_TRUE(cfg.error());
}

TEST(JSON, parse_incorrect5) {
    CommentDesign cd;
    Config cfg = ParseJson("{a=[\"a\naaa\", b],{c=[d,{e=f}]},g]}", cd);
    EXPECT_TRUE(cfg.error());
}

TEST(JSON, parse_correct) {
    Config cfg = ParseJson("{a={b=\"c\nccc\",d=e},f:[{g=[h,{i=j}]}]}");
    EXPECT_FALSE(cfg.error());
}

//пустой ключ (full JSON)
TEST(JSON, parse_without_key) {
    Config cfg = ParseJson("{=b}");
    ASSERT_FALSE(cfg.error());
    EXPECT_EQ(cfg[""], "b");
}

//пустой ключ (simple value)
TEST(JSON, parse_without_key2) {
    Config cfg = ParseJson(":b");
    ASSERT_FALSE(cfg.error());
    EXPECT_EQ(cfg[""], "b");
}

//пустое значение (full JSON)
TEST(JSON, parse_without_value) {
    Config cfg = ParseJson("{a=}");
    ASSERT_FALSE(cfg.error());
    EXPECT_EQ(cfg["a"], nullptr);
}

//пустое значение (simple value)
TEST(JSON, parse_without_value2) {
    Config cfg = ParseJson("a:");
    ASSERT_FALSE(cfg.error());
    EXPECT_EQ(cfg["a"], nullptr);
}

TEST(JSON, parse_simple_value_empty) {
    Config cfg = ParseJson("    ");
    ASSERT_FALSE(cfg.error());
    EXPECT_EQ(cfg, nullptr);
}

TEST(JSON, parse_error_key_value_separator) {
    Config cfg = ParseJson("{a b}");
    ASSERT_TRUE(cfg.error());
    EXPECT_TRUE(RegexCheckCounter(cfg.getError(), 0, 3));
    EXPECT_TRUE(cfg.getError().find("key-value separator") != std::string::npos);
}

TEST(JSON, parse_error_separator) {
    Config cfg = ParseJson("{a=b c=d}");
    ASSERT_TRUE(cfg.error());
    EXPECT_TRUE(RegexCheckCounter(cfg.getError(), 0, 5));
    EXPECT_TRUE(cfg.getError().find("not found end of JSON") != std::string::npos);
    EXPECT_TRUE(cfg.getError().find("value separator") != std::string::npos);
}

TEST(JSON, parse_error_not_found_stop) {
    Config cfg = ParseJson("{a=b ");
    ASSERT_TRUE(cfg.error());
    EXPECT_TRUE(cfg.getError().find("not found end of JSON") != std::string::npos);
}

TEST(JSON, reuse_key) {
    Config cfg;
    cfg.push_back("a", "b");
    cfg.push_back("a", "c");

    EXPECT_EQ(cfg["a"], "c");
}

TEST(JSON, parse_string) {
    std::string input = "{ hw_id=136\n"
            "radio_id=[1, 2] \n"
            "radio_id=3 }";
    Config cfg = ParseJson(input);

    ASSERT_TRUE(cfg.isJson());
    EXPECT_EQ(cfg.size(), 2);
    EXPECT_EQ(cfg[1], 3);
}

TEST(JSON, big_integral_number_to_string) {
    Config cfg;
    cfg.push_back("a", 123456789012345);

    EXPECT_EQ(cfg["a"].toString(), "123456789012345");
}

TEST(JSON, big_float_number_to_string) {
    Config cfg;
    cfg.push_back("a", 123456789012345.4000002L);

    // ввиду особенностей хранения больших дробных чисел, часть вводимой информации может оказаться избыточной
    // в конечном итоге в перменную числа запишется только некоторая часть дроби - знаки после .4 пропадут уже при хранении такого числа
    EXPECT_EQ(cfg["a"].toString(), "123456789012345.4");
}

TEST(JSON, parse_write_infinity) {
    Config cfg;
    cfg.parseJson("{\"key\" : \"inf\" }");
    EXPECT_EQ(cfg["key"].getType(), ValueType::eString);
    EXPECT_EQ(cfg.toString(), "{\"key\":\"inf\"}");

    cfg.parseJson("{\"key\" : \"infinity\" }");
    EXPECT_EQ(cfg["key"].getType(), ValueType::eString);
    EXPECT_EQ(cfg.toString(), "{\"key\":\"infinity\"}");

    cfg.parseJson("{\"key\" : inf }");
    EXPECT_EQ(cfg["key"].getType(), ValueType::eString);
    EXPECT_EQ(cfg.toString(), "{\"key\":\"inf\"}");

    cfg.parseJson("{\"key\" : infinity }");
    EXPECT_EQ(cfg["key"].getType(), ValueType::eString);
    EXPECT_EQ(cfg.toString(), "{\"key\":\"infinity\"}");
}

TEST(JSON, parse_write_negative_infinity) {
    Config cfg;
    cfg.parseJson("{\"a\" : \"-inf\" }");

    EXPECT_EQ(cfg["a"].toString(), "-inf");
}

TEST(JSON, write_infinities) {
    Config cfg;
    cfg["a"] = std::numeric_limits<long double>::infinity();
    cfg["b"] = -std::numeric_limits<long double>::infinity();
    std::string test = cfg.toString();

    EXPECT_EQ(test, "{\"a\":\"inf\",\"b\":\"-inf\"}");
}
