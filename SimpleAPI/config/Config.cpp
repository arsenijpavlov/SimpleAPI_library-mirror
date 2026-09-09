#include "Config.h"

#include "ConfigDefines.h"
#include "ElementNull.h"
#include "ElementBool.h"
#include "ElementNumber.h"
#include "ElementArray.h"
#include "ElementJson.h"
//#include "ElementYaml.h"
#include "ElementXml.h"
#include "../utils/Stacker.h"
#include "../utils/string_utils.h"
#include "../utils/number_utils.h"

#include <limits>
#include <regex>
#include <fstream>
#include <errno.h>


namespace simpleapi {

//Вспомогательная функция
bool WriteStringToFile(const std::string& file_path, std::string&& content) noexcept
{
    std::ofstream file;
    file.open(file_path, std::ios::out);
    if(!file.is_open()) {
        std::cerr << "cannot create file: \"" << file_path << "\"" << std::endl;
        return false;
    }

    try {
        file.write(content.c_str(), content.length());
    } catch(std::exception& e) {
        std::cerr << "exception while \"" << file_path << "\" writing: " << strerror(errno) << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}


void Config::release() noexcept {
    if(m_value)
    {
        delete m_value;
        m_value = nullptr;
    }
}

Config &Config::setComment(const Comment &content) noexcept {
    if(m_value)
        m_value->setComment(content);
    return *this;
}

Config &Config::setComment(const std::string &content_before, const std::string &content_after) noexcept {
    if(m_value)
        m_value->setComment(content_before, content_after);
    return *this;
}

Config &Config::setPrefixComment(const std::string &content) noexcept {
    if(m_value)
        m_value->setPrefixComment(content);
    return *this;
}

Config &Config::setSuffixComment(const std::string &content) noexcept {
    if(m_value)
        m_value->setSuffixComment(content);
    return *this;
}

Config &Config::clearComment() noexcept {
    if(m_value)
        m_value->clearComment();
    return *this;
}

Config &Config::clearPrefixComment() noexcept {
    if(m_value)
        m_value->clearPrefixComment();
    return *this;
}

Config &Config::clearSuffixComment() noexcept {
    if(m_value)
        m_value->clearSuffixComment();
    return *this;
}

Config &Config::deleteComment() noexcept {
    if(m_value)
        m_value->deleteComment();
    return *this;
}

Config &Config::deletePrefixComment() noexcept {
    if(m_value)
        m_value->deletePrefixComment();
    return *this;
}

Config &Config::deleteSuffixComment() noexcept {
    if(m_value)
        m_value->deleteSuffixComment();
    return *this;
}

CommentDesign &Config::getCommentDesign() noexcept
{
    return m_value->getCommentDesign();
}

const CommentDesign &Config::getCommentDesign() const noexcept
{
    return m_value ? m_value->getCommentDesign() : Comment::k_default_design;
}

Config &Config::setCommentDesign(const CommentDesign &design) noexcept {
    if(m_value)
        m_value->setCommentDesign(design);
    return *this;
}

Config &Config::clearCommentDesign() noexcept {
    if(m_value)
        m_value->clearCommentDesign();
    return *this;
}

Config &Config::set_comment(const size_t& index, const Comment &content) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->set_comment(index, content);
    return *this;
}

Config &Config::set_comment(const std::string &key, const Comment &content) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->set_comment(key, content);
    return *this;
}

Config &Config::set_comment(const size_t& index, const std::string &content_before,
                            const std::string &content_after)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->set_comment(index, content_before, content_after);
    return *this;
}

Config &Config::set_comment(const std::string &key, const std::string &content_before,
                            const std::string &content_after)
{
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->set_comment(key, content_before, content_after);
    return *this;
}

Config &Config::set_prefix_comment(const size_t& index, const std::string &content) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->set_prefix_comment(index, content);
    return *this;
}

Config &Config::set_prefix_comment(const std::string &key, const std::string &content) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->set_prefix_comment(key, content);
    return *this;
}

Config &Config::set_suffix_comment(const size_t& index, const std::string &content) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->set_suffix_comment(index, content);
    return *this;
}

Config &Config::set_suffix_comment(const std::string &key, const std::string &content) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->set_suffix_comment(key, content);
    return *this;
}

Comment &Config::get_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    return reinterpret_cast<tools::IElementContainer*>(m_value)->get_comment(index);
}

Comment &Config::get_comment(const std::string &key)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    return reinterpret_cast<tools::ElementJson*>(m_value)->get_comment(key);
}

Comment Config::get_comment(const size_t& index) const {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    return reinterpret_cast<tools::IElementContainer*>(m_value)->get_comment(index);
}

Comment Config::get_comment(const std::string &key) const
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    return reinterpret_cast<tools::ElementJson*>(m_value)->get_comment(key);
}

std::string Config::get_prefix_comment(const size_t& index) const {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    return reinterpret_cast<tools::IElementContainer*>(m_value)->get_prefix_comment(index);
}

std::string Config::get_prefix_comment(const std::string &key) const {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    return reinterpret_cast<tools::ElementJson*>(m_value)->get_prefix_comment(key);
}

//std::string &Config::get_prefix_comment(const size_t& index) {
//    __CHECK_TYPE_IS_CONTAINER__((*this))
//    return reinterpret_cast<tools::IElementContainer*>(m_value)->get_prefix_comment(index);
//}

//std::string &Config::get_prefix_comment(const std::string &key) {
//    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
//    return reinterpret_cast<tools::ElementJson*>(m_value)->get_prefix_comment(key);
//}

std::string Config::get_suffix_comment(const size_t& index) const {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    return reinterpret_cast<tools::IElementContainer*>(m_value)->get_suffix_comment(index);
}

std::string Config::get_suffix_comment(const std::string &key) const {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    return reinterpret_cast<tools::ElementJson*>(m_value)->get_suffix_comment(key);
}

//std::string &Config::get_suffix_comment(const size_t& index){
//    __CHECK_TYPE_IS_CONTAINER__((*this))
//    return reinterpret_cast<tools::IElementContainer*>(m_value)->get_suffix_comment(index);
//}

//std::string &Config::get_suffix_comment(const std::string &key) {
//    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
//    return reinterpret_cast<tools::ElementJson*>(m_value)->get_suffix_comment(key);
//}

Config &Config::clear_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->clear_comment(index);
    return *this;
}

Config &Config::clear_comment(const std::string &key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->clear_comment(key);
    return *this;
}

Config &Config::clear_prefix_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->clear_prefix_comment(index);
    return *this;
}

Config &Config::clear_prefix_comment(const std::string &key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->clear_prefix_comment(key);
    return *this;
}

Config &Config::clear_suffix_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->clear_suffix_comment(index);
    return *this;
}

Config &Config::clear_suffix_comment(const std::string &key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->clear_suffix_comment(key);
    return *this;
}

Config &Config::delete_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->delete_comment(index);
    return *this;
}

Config &Config::delete_comment(const std::string &key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->delete_comment(key);
    return *this;
}

Config &Config::delete_prefix_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->delete_prefix_comment(index);
    return *this;
}

Config &Config::delete_prefix_comment(const std::string &key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->delete_prefix_comment(key);
    return *this;
}

Config &Config::delete_suffix_comment(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    reinterpret_cast<tools::IElementContainer*>(m_value)->delete_suffix_comment(index);
    return *this;
}

Config &Config::delete_suffix_comment(const std::string &key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    reinterpret_cast<tools::ElementJson*>(m_value)->delete_suffix_comment(key);
    return *this;
}

Config &Config::setValue() noexcept {
    release();
    m_value = new tools::ElementNull();
    return *this;
}

//полностью дублирует метод выше, но с передачей аргумента
Config &Config::setValue(std::nullptr_t) noexcept
{
    return setValue();
}

Config &Config::setValue(const Config &other) noexcept {
    using namespace tools;

    if(this != &other)
    {
        release();
        switch(other.getType()) {
        case ValueType::eNull:      {
            setValue();
            if(other.error()) setError(other.getError());
            break;
        }
        case ValueType::eBool:      { setValue(dynamic_cast<const ElementBool*>(other.m_value)->getValue());    break;  }
        case ValueType::eNumber:    { setValue(dynamic_cast<const ElementNumber*>(other.m_value)->getValue());  break;  }
        case ValueType::eChar:      { setValue(dynamic_cast<const ElementChar*>(other.m_value)->getValue());    break;  }
        case ValueType::eString:    { setValue(dynamic_cast<const ElementString*>(other.m_value)->getValue());  break;  }
        case ValueType::eArray:     { setValue(dynamic_cast<const ElementArray&>(*other.m_value));              break;  }
        case ValueType::eJson:      { setValue(dynamic_cast<const ElementJson&>(*other.m_value));               break;  }
        default: break;
        }

        setCommentDesign(other.getCommentDesign());
        setComment(other.getComment());
    }

    return *this;
}

Config &Config::setValue(Config &&other) noexcept {
    using namespace tools;

    if(this != &other)
    {
        // освобождаем прежде занятое значение
        if(m_value)
            delete m_value;

        // передача указателя в текущий объект
        m_value       = other.m_value;
        other.m_value = nullptr;
    }
    return *this;
}

Config &Config::setValue(const tools::IElement &other) noexcept {
    using namespace tools;

    if(this->m_value == &other)
        return *this;

    release();
    switch(other.getType()) {
    case ValueType::eNull:      { setValue();                                                       break;  }
    case ValueType::eBool:      { setValue(dynamic_cast<const ElementBool*>(&other)->getValue());   break;  }
    case ValueType::eNumber:    { setValue(dynamic_cast<const ElementNumber*>(&other)->getValue()); break;  }
    case ValueType::eChar:      { setValue(dynamic_cast<const ElementChar*>(&other)->getValue());   break;  }
    case ValueType::eString:    { setValue(dynamic_cast<const ElementString*>(&other)->getValue()); break;  }
    case ValueType::eArray:     { setValue(dynamic_cast<const ElementArray&>(other));               break;  }
    case ValueType::eJson:      { setValue(dynamic_cast<const ElementJson&>(other));                break;  }
    default:                    break;
    }

    setComment(other.getComment());

    return *this;
}

Config &Config::setValue(tools::IElement &&other) noexcept {
    using namespace tools;

    if(this->m_value == &other)
        return *this;

    release();
    switch(other.getType()) {
    case ValueType::eNull:      { setValue();                                                                   break;  }
    case ValueType::eBool:      { setValue(dynamic_cast<const ElementBool*>(&other)->getValue());               break;  }
    case ValueType::eNumber:    { setValue(std::move(dynamic_cast<const ElementNumber*>(&other)->getValue()));  break;  }
    case ValueType::eChar:      { setValue(std::move(dynamic_cast<const ElementChar*>(&other)->getValue()));    break;  }
    case ValueType::eString:    { setValue(std::move(dynamic_cast<const ElementString*>(&other)->getValue()));  break;  }
    case ValueType::eArray:     { setValue(std::move(dynamic_cast<ElementArray&&>(other)));                     break;  }
    case ValueType::eJson:      { setValue(std::move(dynamic_cast<ElementJson&&>(other)));                      break;  }
    default:                    break;
    }

    setComment(other.getComment());

    return *this;
}

Config &Config::setValue(const bool other) noexcept {
    release();
    m_value = new tools::ElementBool(other);
    return *this;
}

Config &Config::setValue(const long double &other) noexcept {
    release();
    m_value = new tools::ElementNumber(other);
    return *this;
}

Config &Config::setValue(long double &&other) noexcept {
    release();
    m_value = new tools::ElementNumber(std::move(other));
    return *this;
}

Config &Config::setValue(char other) noexcept
{
    release();
    m_value = new tools::ElementChar(other);
    return *this;
}

Config &Config::setValue(const std::string &other) noexcept {
    release();
    m_value = new tools::ElementString(other);
    return *this;
}

Config &Config::setValue(std::string &&other) noexcept {
    release();
    m_value = new tools::ElementString(std::move(other));
    return *this;
}

Config &Config::setValue(const tools::ElementArray &other) noexcept {
    release();
    m_value = new tools::ElementArray(other);
    return *this;
}

Config &Config::setValue(tools::ElementArray &&other) noexcept {
    release();
    m_value = new tools::ElementArray(std::move(other));
    return *this;
}

Config &Config::setValue(const tools::ElementJson &other) noexcept {
    release();
    m_value = new tools::ElementJson(other);
    return *this;
}

Config &Config::setValue(tools::ElementJson &&other) noexcept {
    release();
    m_value = new tools::ElementJson(std::move(other));
    return *this;
}

Config &Config::set(const std::string &key, const Config &value)
{
    insert_at(key, value);
    return *this;
}

Config &Config::set(const std::string &key, Config &&value)
{
    insert_at(key, std::move(value));
    return *this;
}

Config &Config::set(const size_t& index, const Config &value)
{
    insert_at(index, value);
    return *this;
}

Config &Config::set(const size_t& index, Config &&value)
{
    insert_at(index, std::move(value));
    return *this;
}

Config &Config::set(const size_t& index, const std::string &key, const Config &value)
{
    insert_at(index, key, value);
    return *this;
}

Config &Config::set(const size_t& index, const std::string &key, Config &&value)
{
    insert_at(index, key, std::move(value));
    return *this;
}

bool &Config::getBool() {
    __CHECK_TYPE_IS_BOOL__((*this))
    return dynamic_cast<tools::ElementBool*>(m_value)->getValue();
}

// @TEST(ELEMENT, get_const_bool)
bool Config::getBool() const {
    __CHECK_TYPE_IS_CONVERIBLE_TO_BOOL__((*this))

    switch (getType()) {
    case ValueType::eNumber: {
        return dynamic_cast<const tools::ElementNumber*>(m_value)->getValue() > 0;
    }
    case ValueType::eChar:   {
        return utils::CreateBoolFromString(std::string(1, dynamic_cast<const tools::ElementChar*>(m_value)->getValue()));
    }
    case ValueType::eString: {
        return utils::CreateBoolFromString(dynamic_cast<const tools::ElementString*>(m_value)->getValue());
    }
    case ValueType::eBool:
    default: break;
    }
    return dynamic_cast<const tools::ElementBool*>(m_value)->getValue();
}

long double& Config::getNumber() {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return dynamic_cast<tools::ElementNumber*>(m_value)->getValue();
}

long double Config::getNumber() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return dynamic_cast<const tools::ElementNumber*>(m_value)->getValue();
}

uint8_t Config::getUInt8() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<uint8_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

uint16_t Config::getUInt16() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<uint16_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

uint32_t Config::getUInt32() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<int32_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

uint64_t Config::getUInt64() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<uint64_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

int8_t Config::getInt8() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<int8_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

int16_t Config::getInt16() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<int16_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

int32_t Config::getInt32() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<int32_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

int64_t Config::getInt64() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<int64_t>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

float Config::getFloat() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<float>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

double Config::getDouble() const {
    __CHECK_TYPE_IS_NUMBER__((*this))
    return static_cast<double>(dynamic_cast<const tools::ElementNumber*>(m_value)->getValue());
}

long double Config::getLDouble() const {
    return getNumber();
}

std::string &Config::getString() {
    __CHECK_TYPE_IS_STRING__((*this))
    return dynamic_cast<tools::ElementString*>(m_value)->getValue();
}

std::string Config::getString() const {
    __CHECK_TYPE_IS_STRING__((*this))
    return dynamic_cast<const tools::ElementString*>(m_value)->getValue();
}

char &Config::getChar()
{
    __CHECK_TYPE_IS_CHAR_OR_STRING__((*this))

    switch(getType()) {
        case ValueType::eChar: {
            return dynamic_cast<tools::ElementChar*>(m_value)->getValue();
        }
        case ValueType::eString: {
            std::string& str = dynamic_cast<tools::ElementString*>(m_value)->getValue();
            if(str.empty()) str.append(0);
            return str.at(0);
        }
        default: break;
    }

    // до этой строки никогда не доходим (расчёт на __CHECK... проверки)
    return dynamic_cast<tools::ElementChar*>(m_value)->getValue();
}

char Config::getChar() const
{
    __CHECK_TYPE_IS_CHAR_OR_STRING__((*this))

    switch(getType()) {
    case ValueType::eChar: {
            return dynamic_cast<const tools::ElementChar*>(m_value)->getValue();
    }
    case ValueType::eString: {
            std::string str = dynamic_cast<const tools::ElementString*>(m_value)->getValue();
            if(str.empty()) return 0;
            return str.at(0);
    }
    default: break;
    }

    // до этой строки никогда не доходим (расчёт на __CHECK... проверки)
    return dynamic_cast<const tools::ElementChar*>(m_value)->getValue();
}

bool Config::error() const noexcept {
    using namespace tools;

    IErrorField* ptr = dynamic_cast<IErrorField*>(m_value);
    if(ptr == nullptr)
        return false;
    return ptr->error();
}

std::string Config::getError() const noexcept {
    using namespace tools;

    IErrorField* ptr = dynamic_cast<IErrorField*>(m_value);
    if(ptr == nullptr)
        return "";
    return ptr->getError();
}

void Config::setError() noexcept
{
    using namespace tools;

    if(!m_value)
        *this = setValue();
    return m_value->setError();
}

void Config::setError(const std::string &error_string) noexcept
{
    using namespace tools;

    if(!m_value)
        *this = setValue();
    return m_value->setError(error_string);
}

void Config::setError(std::string &&error_string) noexcept
{
    using namespace tools;

    if(!m_value)
        *this = setValue();
    return m_value->setError(std::move(error_string));
}

Config &Config::get_front() {
    using namespace tools;

    __CHECK_TYPE_IS_CONTAINER__((*this))
    switch(getType()) {
    case ValueType::eArray: return dynamic_cast<ElementArray*>(m_value)->get_front();
    case ValueType::eJson:  return dynamic_cast<ElementJson*>(m_value)->get_front();
    default:                break;
    }

    return *this;
}

Config Config::get_front() const {
    using namespace tools;

    __CHECK_TYPE_IS_CONTAINER__((*this))
    switch(getType()) {
    case ValueType::eArray: return dynamic_cast<const ElementArray*>(m_value)->get_front();
    case ValueType::eJson:  return dynamic_cast<const ElementJson*>(m_value)->get_front();
    default:                break;
    }

    return *this;
}

Config &Config::get_at(const size_t& index) {
    using namespace tools;

    __CHECK_TYPE_IS_CONTAINER__((*this))
    switch(getType()) {
    case ValueType::eArray: return dynamic_cast<ElementArray*>(m_value)->get_at(index);
    case ValueType::eJson:  return dynamic_cast<ElementJson*>(m_value)->get_at(index);
    default:                break;
    }

    return *this;
}

Config Config::get_at(const size_t& index) const {
    using namespace tools;

    __CHECK_TYPE_IS_CONTAINER__((*this))
    switch(getType()) {
    case ValueType::eArray: return dynamic_cast<const ElementArray*>(m_value)->get_at(index);
    case ValueType::eJson:  return dynamic_cast<const ElementJson*>(m_value)->get_at(index);
    default:                break;
    }

    return *this;
}

Config &Config::get_at(const std::string& key) {
    try_convert_null_to_json();

    __CHECK_TYPE_IS_JSON__((*this))
    return dynamic_cast<tools::ElementJson*>(m_value)->get_at(key);
}

Config Config::get_at(const std::string& key) const {
    __CHECK_TYPE_IS_JSON__((*this))
    return dynamic_cast<const tools::ElementJson*>(m_value)->get_at(key);
}

Config &Config::get_at(const std::vector<OnlySizetOrString> &complex_key)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config* ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:
        ret = &get_at(complex_key[0].getStringValue());
        break;
    case OnlySizetOrString::Type::type_sizet:
        ret = &get_at(complex_key[0].getIndexValue());
        break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return *ret;

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret->get_at(new_key);
}

Config Config::get_at(const std::vector<OnlySizetOrString> &complex_key) const
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:
        ret = get_at(complex_key[0].getStringValue());
        break;
    case OnlySizetOrString::Type::type_sizet:
        ret = get_at(complex_key[0].getIndexValue());
        break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return ret;

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret.get_at(new_key);
}

Config &Config::get_back() {
    using namespace tools;

    __CHECK_TYPE_IS_CONTAINER__((*this))
    switch(getType()) {
    case ValueType::eArray: return dynamic_cast<ElementArray*>(m_value)->get_back();
    case ValueType::eJson:  return dynamic_cast<ElementJson*>(m_value)->get_back();
    default:                break;
    }

    return *this;
}

Config Config::get_back() const {
    using namespace tools;

    __CHECK_TYPE_IS_CONTAINER__((*this))
    switch(getType()) {
    case ValueType::eArray: return dynamic_cast<const ElementArray*>(m_value)->get_back();
    case ValueType::eJson:  return dynamic_cast<const ElementJson*>(m_value)->get_back();
    default:                break;
    }

    return *this;
}

bool &Config::get_front_bool() {
    Config& config = get_front();
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<tools::ElementBool*>(config.m_value)->getValue();
}

bool Config::get_front_bool() const {
    const Config& config = get_front();
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<const tools::ElementBool*>(config.m_value)->getValue();
}

long double &Config::get_front_number() {
    Config& config = get_front();
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<tools::ElementNumber*>(config.m_value)->getValue();
}

long double Config::get_front_number() const {
    const Config& config = get_front();
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<const tools::ElementNumber*>(config.m_value)->getValue();
}

uint8_t Config::get_front_UInt8() const {
    return static_cast<uint8_t>(get_front_number());
}

uint16_t Config::get_front_UInt16() const {
    return static_cast<uint16_t>(get_front_number());
}

uint32_t Config::get_front_UInt32() const {
    return static_cast<uint32_t>(get_front_number());
}

uint64_t Config::get_front_UInt64() const {
    return static_cast<uint64_t>(get_front_number());
}

int8_t Config::get_front_Int8() const {
    return static_cast<int8_t>(get_front_number());
}

int16_t Config::get_front_Int16() const {
    return static_cast<int16_t>(get_front_number());
}

int32_t Config::get_front_Int32() const {
    return static_cast<int32_t>(get_front_number());
}

int64_t Config::get_front_Int64() const {
    return static_cast<int64_t>(get_front_number());
}

float Config::get_front_Float() const {
    return static_cast<float>(get_front_number());
}

double Config::get_front_Double() const {
    return static_cast<double>(get_front_number());
}

long double Config::get_front_LDouble() const {
    return get_front_number();
}

char &Config::get_front_char()
{
    Config& config = get_front();
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<tools::ElementChar*>(config.m_value)->getValue();
}

char Config::get_front_char() const
{
    const Config& config = get_front();
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<const tools::ElementChar*>(config.m_value)->getValue();
}

std::string &Config::get_front_string() {
    Config& config = get_front();
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<tools::ElementString*>(config.m_value)->getValue();
}

std::string Config::get_front_string() const {
    const Config& config = get_front();
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<const tools::ElementString*>(config.m_value)->getValue();
}

bool &Config::get_bool_at(const size_t& index) {
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<tools::ElementBool*>(config.m_value)->getValue();
}

bool Config::get_bool_at(const size_t& index) const {
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<const tools::ElementBool*>(config.m_value)->getValue();
}

bool &Config::get_bool_at(const std::string& key) {
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<tools::ElementBool*>(config.m_value)->getValue();
}

bool Config::get_bool_at(const std::string& key) const {
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<const tools::ElementBool*>(config.m_value)->getValue();
}

bool &Config::get_bool_at(const std::vector<OnlySizetOrString> &complex_key)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_bool_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config* ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = &get_at(complex_key[0].getStringValue()); break;
    case OnlySizetOrString::Type::type_sizet:   ret = &get_at(complex_key[0].getIndexValue());  break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return (*ret).getBool();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret->get_bool_at(new_key);
}

bool Config::get_bool_at(const std::vector<OnlySizetOrString> &complex_key) const
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_bool_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = get_at(complex_key[0].getStringValue());  break;
    case OnlySizetOrString::Type::type_sizet:   ret = get_at(complex_key[0].getIndexValue());   break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return ret.getBool();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret.get_bool_at(new_key);
}

long double &Config::get_number_at(const size_t& index) {
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<tools::ElementNumber*>(config.m_value)->getValue();
}

long double Config::get_number_at(const size_t& index) const {
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<const tools::ElementNumber*>(config.m_value)->getValue();
}

uint8_t Config::get_UInt8_at(const size_t& index) const {
    return static_cast<uint8_t>(get_number_at(std::move(index)));
}

uint16_t Config::get_UInt16_at(const size_t& index) const {
    return static_cast<uint16_t>(get_number_at(std::move(index)));
}

uint32_t Config::get_UInt32_at(const size_t& index) const {
    return static_cast<uint32_t>(get_number_at(std::move(index)));
}

uint64_t Config::get_UInt64_at(const size_t& index) const {
    return static_cast<uint64_t>(get_number_at(std::move(index)));
}

int8_t Config::get_Int8_at(const size_t& index) const {
    return static_cast<int8_t>(get_number_at(std::move(index)));
}

int16_t Config::get_Int16_at(const size_t& index) const {
    return static_cast<int16_t>(get_number_at(std::move(index)));
}

int32_t Config::get_Int32_at(const size_t& index) const {
    return static_cast<int32_t>(get_number_at(std::move(index)));
}

int64_t Config::get_Int64_at(const size_t& index) const {
    return static_cast<int64_t>(get_number_at(std::move(index)));
}

float Config::get_Float_at(const size_t& index) const {
    return static_cast<float>(get_number_at(std::move(index)));
}

double Config::get_Double_at(const size_t& index) const {
    return static_cast<double>(get_number_at(std::move(index)));
}

long double Config::get_LDouble_at(const size_t& index) const {
    return get_number_at(std::move(index));
}

long double &Config::get_number_at(const std::string& key) {
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<tools::ElementNumber*>(config.m_value)->getValue();
}

long double Config::get_number_at(const std::string& key) const {
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<const tools::ElementNumber*>(config.m_value)->getValue();
}

uint8_t Config::get_UInt8_at(const std::string& key) const {
    return static_cast<uint8_t>(get_number_at(std::move(key)));
}

uint16_t Config::get_UInt16_at(const std::string& key) const {
    return static_cast<uint16_t>(get_number_at(std::move(key)));
}

uint32_t Config::get_UInt32_at(const std::string& key) const {
    return static_cast<uint32_t>(get_number_at(std::move(key)));
}

uint64_t Config::get_UInt64_at(const std::string& key) const {
    return static_cast<uint64_t>(get_number_at(std::move(key)));
}

int8_t Config::get_Int8_at(const std::string& key) const {
    return static_cast<int8_t>(get_number_at(std::move(key)));
}

int16_t Config::get_Int16_at(const std::string& key) const {
    return static_cast<int16_t>(get_number_at(std::move(key)));
}

int32_t Config::get_Int32_at(const std::string& key) const {
    return static_cast<int32_t>(get_number_at(std::move(key)));
}

int64_t Config::get_Int64_at(const std::string& key) const {
    return static_cast<int64_t>(get_number_at(std::move(key)));
}

float Config::get_Float_at(const std::string& key) const {
    return static_cast<float>(get_number_at(std::move(key)));
}

double Config::get_Double_at(const std::string& key) const {
    return static_cast<double>(get_number_at(std::move(key)));
}

long double Config::get_LDouble_at(const std::string& key) const {
    return get_number_at(std::move(key));
}

long double &Config::get_number_at(const std::vector<OnlySizetOrString> &complex_key)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_number_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config* ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = &get_at(complex_key[0].getStringValue()); break;
    case OnlySizetOrString::Type::type_sizet:   ret = &get_at(complex_key[0].getIndexValue());  break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return (*ret).getNumber();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret->get_number_at(new_key);
}

long double Config::get_number_at(const std::vector<OnlySizetOrString> &complex_key) const
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_number_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = get_at(complex_key[0].getStringValue());  break;
    case OnlySizetOrString::Type::type_sizet:   ret = get_at(complex_key[0].getIndexValue());   break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return ret.getNumber();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret.get_number_at(new_key);
}

uint8_t Config::get_UInt8_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<uint8_t>(get_number_at(std::move(complex_key)));
}

uint16_t Config::get_UInt16_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<uint16_t>(get_number_at(std::move(complex_key)));
}

uint32_t Config::get_UInt32_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<uint32_t>(get_number_at(std::move(complex_key)));
}

uint64_t Config::get_UInt64_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<uint64_t>(get_number_at(std::move(complex_key)));
}

int8_t Config::get_Int8_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<int8_t>(get_number_at(std::move(complex_key)));
}

int16_t Config::get_Int16_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<int16_t>(get_number_at(std::move(complex_key)));
}

int32_t Config::get_Int32_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<int32_t>(get_number_at(std::move(complex_key)));
}

int64_t Config::get_Int64_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<int64_t>(get_number_at(std::move(complex_key)));
}

float Config::get_Float_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<float>(get_number_at(std::move(complex_key)));
}

double Config::get_Double_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return static_cast<double>(get_number_at(std::move(complex_key)));
}

long double Config::get_LDouble_at(const std::vector<OnlySizetOrString> &complex_key) const {
    return get_number_at(std::move(complex_key));
}

long double &Config::get_number_at(const std::initializer_list<OnlySizetOrString> &complex_key) {
    return get_number_at(std::vector<OnlySizetOrString>(complex_key));
}

long double Config::get_number_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return get_number_at(std::vector<OnlySizetOrString>(complex_key));
}

uint8_t Config::get_UInt8_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<uint8_t>(get_number_at(std::move(complex_key)));
}

uint16_t Config::get_UInt16_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<uint16_t>(get_number_at(std::move(complex_key)));
}

uint32_t Config::get_UInt32_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<uint32_t>(get_number_at(std::move(complex_key)));
}

uint64_t Config::get_UInt64_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<uint64_t>(get_number_at(std::move(complex_key)));
}

int8_t Config::get_Int8_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<int8_t>(get_number_at(std::move(complex_key)));
}

int16_t Config::get_Int16_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<int16_t>(get_number_at(std::move(complex_key)));
}

int32_t Config::get_Int32_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<int32_t>(get_number_at(std::move(complex_key)));
}

int64_t Config::get_Int64_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<int64_t>(get_number_at(std::move(complex_key)));
}

float Config::get_Float_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<float>(get_number_at(std::move(complex_key)));
}

double Config::get_Double_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return static_cast<double>(get_number_at(std::move(complex_key)));
}

long double Config::get_LDouble_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return get_number_at(std::move(complex_key));
}

char &Config::get_char_at(const size_t &index)
{
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<tools::ElementChar*>(config.m_value)->getValue();
}

char Config::get_char_at(const size_t &index) const
{
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<const tools::ElementChar*>(config.m_value)->getValue();
}

char &Config::get_char_at(const std::string &key)
{
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<tools::ElementChar*>(config.m_value)->getValue();
}

char Config::get_char_at(const std::string &key) const
{
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<const tools::ElementChar*>(config.m_value)->getValue();
}

char &Config::get_char_at(const std::vector<OnlySizetOrString> &complex_key)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_char_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config* ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = &get_at(complex_key[0].getStringValue()); break;
    case OnlySizetOrString::Type::type_sizet:   ret = &get_at(complex_key[0].getIndexValue());  break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return (*ret).getChar();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret->get_char_at(new_key);
}

char Config::get_char_at(const std::vector<OnlySizetOrString> &complex_key) const
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_char_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = get_at(complex_key[0].getStringValue());  break;
    case OnlySizetOrString::Type::type_sizet:   ret = get_at(complex_key[0].getIndexValue());   break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return ret.getChar();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret.get_char_at(new_key);
}

char &Config::get_char_at(const std::initializer_list<OnlySizetOrString> &complex_key)
{
    return get_char_at(std::vector<OnlySizetOrString>(complex_key));
}

char Config::get_char_at(const std::initializer_list<OnlySizetOrString> &complex_key) const
{
    return get_char_at(std::vector<OnlySizetOrString>(complex_key));
}

std::string &Config::get_string_at(const size_t& index) {
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<tools::ElementString*>(config.m_value)->getValue();
}

std::string Config::get_string_at(const size_t& index) const {
    const Config& config = get_at(index);
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<const tools::ElementString*>(config.m_value)->getValue();
}

std::string &Config::get_string_at(const std::string& key) {
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<tools::ElementString*>(config.m_value)->getValue();
}

std::string Config::get_string_at(const std::string& key) const {
    const Config& config = get_at(key);
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<const tools::ElementString*>(config.m_value)->getValue();
}

std::string &Config::get_string_at(const std::vector<OnlySizetOrString> &complex_key)
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_string_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config* ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = &get_at(complex_key[0].getStringValue()); break;
    case OnlySizetOrString::Type::type_sizet:   ret = &get_at(complex_key[0].getIndexValue());  break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return (*ret).getString();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret->get_string_at(new_key);
}

std::string Config::get_string_at(const std::vector<OnlySizetOrString> &complex_key) const
{
    __CHECK_TYPE_IS_CONTAINER__((*this))
    if(complex_key.size() == 0)
        throw std::invalid_argument("get_string_at({}) - try a get incorrect index/key (no value)");

    //определить тип значения первого индекса/ключа
    Config ret;
    switch (complex_key[0].getType()) {
    case OnlySizetOrString::Type::type_string:  ret = get_at(complex_key[0].getStringValue());  break;
    case OnlySizetOrString::Type::type_sizet:   ret = get_at(complex_key[0].getIndexValue());   break;
    default:
        //это значение пользователь библиотеки не увидит
        throw std::invalid_argument("incorrect complex_key type");
    }

    if(complex_key.size() == 1)
        return ret.getString();

    //создать следующую итерацию списка
    std::vector<OnlySizetOrString> new_key = complex_key;
    new_key.erase(new_key.cbegin());
    return ret.get_string_at(new_key);
}

std::string &Config::get_string_at(const std::initializer_list<OnlySizetOrString> &complex_key) {
    return get_string_at(std::vector<OnlySizetOrString>(complex_key));
}

std::string Config::get_string_at(const std::initializer_list<OnlySizetOrString> &complex_key) const {
    return get_string_at(std::vector<OnlySizetOrString>(complex_key));
}

bool &Config::get_bool_back() {
    const Config& config = get_back();
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<tools::ElementBool*>(config.m_value)->getValue();
}

bool Config::get_bool_back() const {
    const Config& config = get_back();
    __CHECK_TYPE_IS_BOOL__(config)

    return dynamic_cast<const tools::ElementBool*>(config.m_value)->getValue();
}

long double &Config::get_number_back() {
    const Config& config = get_back();
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<tools::ElementNumber*>(config.m_value)->getValue();
}

long double Config::get_number_back() const {
    const Config& config = get_back();
    __CHECK_TYPE_IS_NUMBER__(config)

    return dynamic_cast<const tools::ElementNumber*>(config.m_value)->getValue();
}

uint8_t Config::get_UInt8_back() const {
    return static_cast<uint8_t>(get_number_back());
}

uint16_t Config::get_UInt16_back() const {
    return static_cast<uint16_t>(get_number_back());
}

uint32_t Config::get_UInt32_back() const {
    return static_cast<uint32_t>(get_number_back());
}

uint64_t Config::get_UInt64_back() const {
    return static_cast<uint64_t>(get_number_back());
}

int8_t Config::get_Int8_back() const {
    return static_cast<int8_t>(get_number_back());
}

int16_t Config::get_Int16_back() const {
    return static_cast<int16_t>(get_number_back());
}

int32_t Config::get_Int32_back() const {
    return static_cast<int32_t>(get_number_back());
}

int64_t Config::get_Int64_back() const {
    return static_cast<int64_t>(get_number_back());
}

float Config::get_Float_back() const {
    return static_cast<float>(get_number_back());
}

double Config::get_Double_back() const {
    return static_cast<double>(get_number_back());
}

long double Config::get_LDouble_back() const {
    return get_number_back();
}

char &Config::get_char_back()
{
    const Config& config = get_back();
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<tools::ElementChar*>(config.m_value)->getValue();
}

char Config::get_char_back() const
{
    const Config& config = get_back();
    __CHECK_TYPE_IS_CHAR__(config)

    return dynamic_cast<const tools::ElementChar*>(config.m_value)->getValue();
}

std::string &Config::get_string_back() {
    const Config& config = get_back();
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<tools::ElementString*>(config.m_value)->getValue();
}

std::string Config::get_string_back() const {
    const Config& config = get_back();
    __CHECK_TYPE_IS_STRING__(config)

    return dynamic_cast<const tools::ElementString*>(config.m_value)->getValue();
}

Config &Config::clearContainer()
{
    __CHECK_TYPE_IS_CONTAINER__((*this))

    dynamic_cast<tools::IElementContainer*>(m_value)->clearContainer();
    return *this;
}

void Config::try_convert_null_to_json() noexcept
{
    using namespace tools;

    if(isNull()) {
        CommentDesign temp_design = getCommentDesign();
        release();
        m_value = new ElementJson();
        setCommentDesign(temp_design);
    }
}

void Config::try_convert_null_to_json_array() noexcept
{
    using namespace tools;

    if(isNull()){
        CommentDesign temp_design = getCommentDesign();
        release();
        m_value = new ElementArray();
        setCommentDesign(temp_design);
    }
}

Config& Config::insert_front(const Config& other) {
    Config config(other);
    return insert_front(std::move(config));
}

Config& Config::insert_front(Config&& other) {
    try_convert_null_to_json_array();

    __CHECK_TYPE_IS_INDEX_CONTAINER__((*this))
    dynamic_cast<tools::ElementArray*>(m_value)->insert_front(std::move(other));

    return *this;
}

Config& Config::insert_front(const std::string& key, const Config& other) {
    Config config(other);
    return insert_front(key, std::move(config));
}

Config& Config::insert_front(const std::string& key, Config&& other) {
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_front(key, std::move(other));

    return *this;
}

Config& Config::insert_at(const size_t& index, const Config& other) {
    Config config(other);
    return insert_at(index, std::move(config));
}

Config& Config::insert_at(const size_t& index, Config&& other) {
    try_convert_null_to_json_array();

    __CHECK_TYPE_IS_INDEX_CONTAINER__((*this))
    dynamic_cast<tools::ElementArray*>(m_value)->insert_at(index, std::move(other));

    return *this;
}

Config& Config::insert_at(const std::string& key, const Config& other) {
    Config config(other);
    return insert_at(key, std::move(config));
}

Config& Config::insert_at(const std::string& key, Config&& other) {
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_at(key, std::move(other));

    return *this;
}

Config &Config::insert_at(const size_t& index, const std::string &key, const Config &other)
{
    Config config(other);
    return insert_at(index, key, std::move(config));
}

Config &Config::insert_at(const size_t& index, const std::string &key, Config &&other)
{
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_at(index, key, std::move(other));

    return *this;
}

Config &Config::insert_at(const shared_VElement::iterator iterator, const Config &other)
{
    Config temp(other);
    return insert_at(iterator, std::move(other));
}

Config &Config::insert_at(const shared_VElement::iterator iterator, Config &&other)
{
    try_convert_null_to_json_array();

    __CHECK_TYPE_IS_INDEX_CONTAINER__((*this))
    dynamic_cast<tools::ElementArray*>(m_value)->insert_at(iterator, std::move(other));

    return *this;
}

Config &Config::insert_at(const shared_VPairElement::iterator iterator, const std::string& key,
                          const Config &other)
{
    Config temp(other);
    return insert_at(iterator, key, std::move(other));
}

Config &Config::insert_at(const shared_VPairElement::iterator iterator, const std::string& key,
                          Config &&other)
{
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_at(iterator, key, std::move(other));

    return *this;
}

Config& Config::insert_back(const Config& other) {
    Config config(other);
    return insert_back(std::move(config));
}

Config& Config::insert_back(Config&& other) {
    try_convert_null_to_json_array();

    __CHECK_TYPE_IS_INDEX_CONTAINER__((*this))
    dynamic_cast<tools::ElementArray*>(m_value)->insert_back(std::move(other));

    return *this;
}

Config& Config::insert_back(const std::string& key, const Config& other) {
    Config config(other);
    return insert_back(key, std::move(config));
}

Config& Config::insert_back(const std::string& key, Config&& other) {
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_back(key, std::move(other));

    return *this;
}

//найти первый попавшийся Json и дополнить его новым элементом (на каждом уровне вложенности)
Config &Config::insert_back_force(const VString &keys, const Config &other) noexcept
{
    Config temp(other);
    return push_back_force(keys, std::move(temp));
}

/* Общая логика работы функции:
 * если вектор ключей пустой
 *  - приравнять или дополнить (преобразовав в массив) текущий объект значением other
 * иначе
 *  - взять первый ключ из вектора
 *  - проверить наличие ключа в текущем объекте, если ключ существует
 *    > если текущий объект не контейнер
 *      >> преобразовать в массив (т.к. у этого значения нет ключа)
 *    > если текущий объект массив
 *      >> найти первый попавшийся Json (создать)
 *    > дополнить итоговый Json новым ключом
 *  - если ключ не существовал - добавить новый ключ со значением Null
 *
 *  Вернёт последнее изменённое значение (ссылку на other)
*/
Config &Config::insert_back_force(const VString &keys, Config &&other) noexcept
{
    if(keys.empty()) {
        switch (getType()) {
        case ValueType::eNull: {
            //значения не было, можно переназначить
            *this = std::move(other);
            return *this;
        }
        default: {
            //преобразовать текущее значение в массив и перейти к дополнению массива новым значением
            Config temp = *this;
            *this = Config(ValueType::eArray, std::move(temp));
            //перейти в управление массивом
        }
        case ValueType::eArray: {
            //дополнить массив новым значением
            insert_back(std::move(other));
            return get_back();
        }
        }
    } else {
        VString new_keys = keys;
        std::string key = new_keys[0];
        new_keys.erase(new_keys.cbegin());

        if(containsKey(key)) {
            //ключ уже существует, передаём управление дальше в рекурсию
            return get_at(key).insert_back_force(new_keys, std::move(other));
        } else {
            Config* found_json = nullptr;

            //на основе типа текущего значения
            switch(getType()) {
            default: {
                //преобразовать текущее значение в массив
                Config temp = *this;
                *this = Config(ValueType::eArray, std::move(temp));
                //перейти в управление как Array
            }
            case ValueType::eArray: {
                //найти в массиве первый попавшийся Json или дополнить вновь созданным
                for(auto& cfg : getRange()) {
                    if(cfg->isJson()) {
                        found_json = cfg.get();
                        break;
                    }
                }

                //дополнить, если Json не найден
                if(!found_json) {
                    insert_back(Config(ValueType::eJson));
                    found_json = &get_back();
                }
                break;
            }
            case ValueType::eNull: {
                //значение создаётся с нуля
                *this = Config(ValueType::eJson);
                //перейти в управление как Json
            }
            case ValueType::eJson: {
                //текущее значене уже является Json
                found_json = this;
                break;
            }
            }

            if(!found_json->containsKey(key)) {
                //дополнить новым ключом
                found_json->insert_back(key, Config());
            }
            return found_json->get_at(key).insert_back_force(new_keys, std::move(other));
        }
    }

    return *this; //сюда попасть не должны
}



Config &Config::insert_before(const std::string &before_key, const std::string &key, const Config &other)
{
    Config config(other);
    return insert_before(before_key, key, std::move(config));
}

Config &Config::insert_before(const std::string &before_key, const std::string &key, Config &&other)
{
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_before(before_key, key, std::move(other));

    return *this;
}

Config& Config::insert_after(const std::string& after_key, const std::string& key, const Config& other) {
    Config config(other);
    return insert_after(after_key, key, std::move(config));
}

Config& Config::insert_after(const std::string& after_key, const std::string& key, Config&& other) {
    try_convert_null_to_json();

    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->insert_after(after_key, key, std::move(other));

    return *this;
}

Config &Config::append(const Config &config) {
    Config cfg_copy(config);
    return append(std::move(cfg_copy));
}

Config &Config::append(Config &&config) {
    if(config.isNull())
        return *this;

    if(!isNull())
    {
        __CHECK_TYPE_IS_CONTAINER__((*this))
        __CHECK_TYPES_IS_EQUAL__((*this), config)

        switch(getType()) {
        case ValueType::eArray: {
            dynamic_cast<tools::ElementArray*>(m_value)->append(dynamic_cast<tools::ElementArray&&>(*config.m_value));
            break;
        }
        case ValueType::eJson: {
            dynamic_cast<tools::ElementJson*>(m_value)->append(dynamic_cast<tools::ElementJson&&>(*config.m_value));
            break;
        }
        default: throw std::invalid_argument("Config::append(): unexpected type of config: " + simpleapi::ToString(config.getType()));
        }
    } else {
        setValue(config);
    }

    return *this;
}

Config& Config::erase_front() {
    erase_at(0);
    return *this;
}

//если элемента не существует - проигнорировать
Config& Config::erase_at(const size_t& index) {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    dynamic_cast<tools::IElementContainer*>(m_value)->erase_at(index);

    return *this;
}

Config& Config::erase_at(const std::string& key) {
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->erase_at(key);

    return *this;
}

Config& Config::erase_back() {
    __CHECK_TYPE_IS_CONTAINER__((*this))
    dynamic_cast<tools::IElementContainer*>(m_value)->erase_back();

    return *this;
}

Config &Config::erase_at(const shared_VElement::iterator iterator)
{
    __CHECK_TYPE_IS_ARRAY__((*this))
    dynamic_cast<tools::ElementArray*>(m_value)->erase_at(iterator);

    return *this;
}

Config &Config::erase_at(const shared_VPairElement::iterator iterator)
{
    __CHECK_TYPE_IS_MAP_CONTAINER__((*this))
    dynamic_cast<tools::ElementJson*>(m_value)->erase_at(iterator);

    return *this;
}

Config Config::get_and_pop_front() {
    Config config = get_front();
    erase_front();

    return config;
}

Config Config::get_and_pop_at(const size_t& index) {
    Config config = get_at(index);
    erase_at(index);

    return config;
}

Config Config::get_and_pop_at(const std::string& key) {
    Config config = get_at(key);
    erase_at(key);

    return config;
}

Config Config::get_and_pop_back() {
    Config config = get_back();
    erase_back();

    return config;
}

// @TEST(ELEMENT, char_compare_string)
bool Config::isEqual(const tools::IElement &other, const bool compare_comments,
                     const bool map_sort_important) const noexcept
{
    __CHECK_TYPES_NOT_EQUAL_ACTION__((*m_value), other)
        return false;

    return m_value->isEqual(other, compare_comments, map_sort_important);
}

bool Config::isEqual(bool other) const noexcept {
    __CHECK_TYPE_NOT_BOOL_ACTION__((*this))
        return false;

    return dynamic_cast<const tools::ElementBool*>(m_value)->getValue() == other;
}

bool Config::isEqual(char other) const noexcept
{
    __CHECK_TYPE_NOT_CHAR_ACTION__((*this))
    return false;

    return dynamic_cast<const tools::ElementBool*>(m_value)->getValue() == other;
}

bool Config::isEqual(const long double &other) const noexcept {
    __CHECK_TYPE_NOT_NUMBER_ACTION__((*this))
        return false;

    return dynamic_cast<const tools::ElementNumber*>(m_value)->getValue() == other;
}

bool Config::isEqual(const std::string &other) const noexcept {
    __CHECK_TYPE_NOT_STRING_ACTION__((*this))
        return false;

    return dynamic_cast<const tools::ElementString*>(m_value)->getValue() == other;
}

bool Config::containsValue(const Config &config) const noexcept {
    switch(getType()) {
    case ValueType::eNull:      return true;
    case ValueType::eBool:      return config.isBool() && getBool() == config.getBool();
    case ValueType::eNumber:    return config.isNumber() && getNumber() == config.getNumber();
    case ValueType::eChar:      return config.isChar() && getChar() == config.getChar();
    case ValueType::eString:    return config.isString() && getString() == config.getString();
    case ValueType::eArray:     return std::any_of(getRange().cbegin(), getRange().cend(),
                                    [&config](const std::shared_ptr<Config>& value)
                                    { return *value == config; });
    case ValueType::eJson:
                                return std::any_of(getNamedRange().cbegin(), getNamedRange().cend(),
                                    [&config](const std::pair<std::string, std::shared_ptr<Config>>& pair)
                                    { return *pair.second == config; });
    case ValueType::eYaml:      // FIXME: return containsValue() in YAML;
    case ValueType::eXml:       // FIXME: return containsValue() in XML;
        break;
    }

    return false;
}

bool Config::containsKey(const std::string &key) const noexcept {
    if(!isMapContainer()) return false;

    return std::any_of(getNamedRange().cbegin(), getNamedRange().cend(),
                       [&key](const std::pair<std::string, std::shared_ptr<Config>>& pair)
                       { return pair.first == key; });
}

shared_VElement::iterator Config::array_begin() {
    __CHECK_TYPE_IS_ARRAY__((*this))
    return dynamic_cast<tools::ElementArray*>(m_value)->begin();
}

shared_VElement::const_iterator Config::array_cbegin() const {
    __CHECK_TYPE_IS_ARRAY__((*this))
    return dynamic_cast<const tools::ElementArray*>(m_value)->cbegin();
}

shared_VElement::iterator Config::array_end() {
    __CHECK_TYPE_IS_ARRAY__((*this))
    return dynamic_cast<tools::ElementArray*>(m_value)->end();
}

shared_VElement::const_iterator Config::array_cend() const {
    __CHECK_TYPE_IS_ARRAY__((*this))
    return dynamic_cast<const tools::ElementArray*>(m_value)->cend();
}

shared_VPairElement::iterator Config::map_begin() {
    __CHECK_TYPE_IS_JSON__((*this))
    return dynamic_cast<tools::ElementJson*>(m_value)->begin();
}

shared_VPairElement::const_iterator Config::map_cbegin() const {
    __CHECK_TYPE_IS_JSON__((*this))
    return dynamic_cast<const tools::ElementJson*>(m_value)->cbegin();
}

shared_VPairElement::iterator Config::map_end() {
    __CHECK_TYPE_IS_JSON__((*this))
    return dynamic_cast<tools::ElementJson*>(m_value)->end();
}

shared_VPairElement::const_iterator Config::map_cend() const {
    __CHECK_TYPE_IS_JSON__((*this))
    return dynamic_cast<const tools::ElementJson*>(m_value)->cend();
}


std::string Config::toString(const ConfigFormat format, const CommentDesign &design,
                             const int8_t tabulation_level) const noexcept
{
    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется собственная
        n_design = m_value->getCommentDesign();
    }

    switch(getType()){
    case ValueType::eArray:
        return dynamic_cast<const tools::ElementArray*>(m_value)->toString(format, n_design, tabulation_level);
    case ValueType::eJson:
        return dynamic_cast<const tools::ElementJson*>(m_value)->toString(format, n_design, tabulation_level);
    default:
        return m_value->toString();
    }
}

std::ostream &operator<<(std::ostream &os, const Config &config) noexcept {
    os << config.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const tools::IElement &config) noexcept {
    os << config.toString();
    return os;
}

bool Config::readFile(const std::string &file_path, const ConfigFormat format,
                      const CommentDesign &design) noexcept
{
    release();
    switch(format) {
    case ConfigFormat::eJSON:   return readFileJson(file_path, design);
    case ConfigFormat::eINI:    return readFileIni(file_path, design);
    // case ConfigFormat::eYAML:
    // case ConfigFormat::eXML:
    default: break;
    }

    setError("unexpected ConfigFormat");
    return false;
}

bool Config::readFileJson(const std::string &file_path, const CommentDesign &design) noexcept
{
    // если переменная не заполнена, используется собственное значение CommentDesign
    CommentDesign n_design = design;
    if(m_value && design == CommentDesign{}) {
        n_design = m_value->getCommentDesign();
    }

    std::string input_str;
    std::string error_str;
    if(GetAllStringsFromFile(file_path, input_str, &error_str)) {
        return parseJson(input_str, n_design);
    }

    setError(error_str);
    return false;
}

bool Config::readFileIni(const std::string &file_path, const CommentDesign &design) noexcept
{
    // если переменная не заполнена, используется собственное значение CommentDesign
    CommentDesign n_design = design;
    if(m_value && design == CommentDesign{}) {
        n_design = m_value->getCommentDesign();
    }

    std::string input_str;
    std::string error_str;
    if(GetAllStringsFromFile(file_path, input_str, &error_str)) {
        return parseIni(input_str, n_design);
    }

    setError(error_str);
    return false;
}

bool Config::readFileXml(const std::string &file_path, const CommentDesign &design) noexcept
{
    //TODO: Config::readFileXml()
    return false;
}

bool Config::writeFile(const std::string &file_path,  const ConfigFormat format,
                       const CommentDesign &design,
                       const int8_t custom_tabulation_level) noexcept
{
    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется собственная
        n_design = m_value->getCommentDesign();
    }

    return WriteFile(*this, file_path, format, n_design, custom_tabulation_level);
}

bool Config::writeFileJson(const std::string &file_path, const CommentDesign &design,
                           const int8_t custom_tabulation_level) noexcept
{
    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется соб
        n_design = m_value->getCommentDesign();
    }

    return WriteFileJson(*this, file_path, n_design, custom_tabulation_level);
}

bool Config::writeFileIni(const std::string &file_path, const CommentDesign &design,
                          const int8_t custom_tabulation_level) noexcept
{
    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется собственная
        n_design = m_value->getCommentDesign();
    }

    return WriteFileIni(*this, file_path, n_design, custom_tabulation_level);
}

bool Config::writeFileXml(const std::string &file_path, const CommentDesign &design, const int8_t custom_tabulation_level) noexcept
{
    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется собственная
        n_design = m_value->getCommentDesign();
    }

    return WriteFileXml(*this, file_path, n_design, custom_tabulation_level);
}

bool Config::parse(const std::string &content, const ConfigFormat format,
                   const CommentDesign &design) noexcept
{
    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется соб
        n_design = m_value->getCommentDesign();
    }

    release();

    switch(format) {
    case ConfigFormat::eJSON:   return parseJson(content, n_design);
    case ConfigFormat::eINI:    return parseIni(content, n_design);
    case ConfigFormat::eYAML:   return parseYaml(content, n_design);
    case ConfigFormat::eXML:    return parseXml(content, n_design);
    default:                    break;
    }

    Config out;
    out.setError("unexpected ConfigFormat");
    return false;
}

bool Config::parseJson(const std::string &content, const CommentDesign &design) noexcept
{
    //определить тип значения верхнего уровня и передать в соответствующий обработчик
    /* игнорируя комментарий, найти первое вхождение символа ключа(значения)
     * определить следующий после "слова" символ-разделитель, если он есть
     *   - комментарий после "слова" тоже игнорируется
     * если за символами пробела есть разделитель key-value - передать на парсер ElementJson::parseJson()
     * если за "пробельными символами" есть новое "слово" - явно ошибка
     * иначе использовать парсер CreateElementFromString(), а комментарии сохранить здесь же
     */
    std::string current_comment;
    VString comments;

    tools::Stacker stacker;
    stacker.addSimpleRule('\'');
    stacker.addSimpleRule('"');
    stacker.addDoubleRule('[', ']');
    stacker.addDoubleRule('{', '}');

    bool value_found           = false;
    size_t value_started_at    = 0;
    bool is_full_json          = false;

    ParserSymbolCounter counter;
    CommentDesign n_design = design; //для изменения в процессе парсинга

    auto CreateError = [&](const ParserSymbolCounter& counter, const std::string& message) -> void {
        setError("ElementJson JSON parser error[" + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorLine = [&](const ParserSymbolCounter& counter, const std::string& message) -> void {
        setError("ElementJson JSON parser error at line [" + std::to_string(counter.getLastLineCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorUnexpected = [&](const ParserSymbolCounter& counter, const char ch, const std::string& message = "") -> void {
        std::string temp = std::string("ElementJson JSON parser: unexpected symbol '") + ch;
        temp += "' at ["
                + std::to_string(counter.getLastLineCounter())
                + "][" + std::to_string(counter.getLastSymbolCounter());
        temp += "]" + (message.empty() ? "" : ": " + message + "!");
        setError(temp);
    };

    //нужно определить тип значения: "100%-json", "json с одним ключом" или что-то иное
    char ch_previous = 0;
    char ch_current = 0;
    for(size_t i = 0; i < content.size(); i++, ch_previous = ch_current) {
        ch_current  = content[i];
        const char ch_next     = i < content.size() ? content[i + 1] : 0;

        counter.check(i, ch_current);

        //поиск комментариев ===================================================
        //вернёт комментарий без обрамления
        try {
            tools::CheckComments(ch_current, ch_next, i, n_design, current_comment, stacker.empty());
        } catch(std::exception& e) {
            //не хватило символов для прочтения комментария
            CreateError(counter, e.what());
            break;
        }

        if(!n_design.with_comments)
            current_comment.clear();
        if(n_design.with_comments && n_design.temp_type == CommentType::eCommentEnd)
        {
            // запомнить комментарий, если он потом понадобится для одиночного значения
            comments.push_back(tools::FromComment(std::move(current_comment), n_design));
            current_comment.clear();
            n_design.temp_type = CommentType::eNotComment;
            continue;
        }
        if(n_design.temp_type != CommentType::eNotComment)
            continue; // здесь комментарий, не обрабатываем символ
        //=================================================== поиск комментариев

        //первый не пробельный символ является определителем
        if(!value_found && stacker.empty() && !utils::CharInString(ch_current, __SPACES__))
        {
            value_found      = true;
            value_started_at = i;
            if(ch_current == '{') {
                is_full_json = true;
                break;
            }
        }

        //находим следующие определители
        if(value_found && stacker.empty() && utils::CharInString(ch_current, "=:")) {
            is_full_json = true;
            break;
        }

        if(ch_previous != '\\') {
            if(!stacker.autocheck(ch_current)) {
                break;
            }
        }
    }

    Config& result_cfg = *this;

    std::string prefix_comment = utils::VStringToString(comments);

    if(is_full_json) {
        // нужно парсить как полноценный Json документ

        result_cfg = Config(ValueType::eJson);
        result_cfg.setCommentDesign(n_design);
        result_cfg.parseFullJsonDoc(std::string(content));
    } else if(value_found) {
        // нужно парсить как одиночное значение
        result_cfg.setCommentDesign(n_design);

        //сначала нужно отделить комментарии, затем обработать внутреннее значение через CreateElementFromString()
        //начальные комментарии уже известны
        //здесь только значение, ключа быть не может
        ParserSymbolCounter start_value_counter; //будет использовано в CreateElementFromString()
        std::string value;

        auto ConfirmValue = [&, content](const bool for_penultimate = false) -> bool {
//            counter.printCoords(); // для отладки
            Config element = CreateElementFromString(std::move(value), ConfigFormat::eJSON,
                                         static_cast<const Config&>(result_cfg).getCommentDesign(),
                                         start_value_counter);
            if(element.error()) {
                //если случилась ошибка при внутренней конвертации прочитанного значения,
                // то эта ошибка становится основной ошибкой парсинга
                CreateError(counter, element.getError());
                return false;
            }

            result_cfg = Config(std::move(element));
            if(result_cfg.getCommentDesign().opt_multiline_column_size > n_design.opt_multiline_column_size)
                n_design.opt_multiline_column_size = result_cfg.getCommentDesign().opt_multiline_column_size;

            return true;
        };

        bool value_stored = false;
        //нужно прочитать значение полностью и отделить его от замыкающего комментария
        for(size_t i = value_started_at; i < content.size(); i++) {
            char ch_previous    = i == 0 ? 0 : content[i - 1];
            char ch_current     = content[i];
            char ch_next        = i < content.size() ? content[i + 1] : 0;

            counter.check(i, ch_current);

            //поиск комментариев ===================================================
            //вернёт комментарий без обрамления
            try {
                tools::CheckComments(ch_current, ch_next, i, n_design, current_comment, stacker.empty());
            } catch(std::exception& e) {
                //не хватило символов для прочтения комментария
                CreateError(counter, e.what());
                break;
            }

            if(!n_design.with_comments)
                current_comment.clear();
            if(n_design.with_comments && n_design.temp_type == CommentType::eCommentEnd)
            {
                comments.push_back(tools::FromComment(std::move(current_comment), n_design));
                current_comment.clear();
                n_design.temp_type = CommentType::eNotComment;
                continue;
            }
            if(n_design.temp_type != CommentType::eNotComment)
                continue;
            //=================================================== поиск комментариев

            if(!value_stored) {
                if(ch_previous != '\\') {
                    if(!stacker.autocheck(ch_current)) {
                        CreateErrorUnexpected(counter, ch_current);
                        break;
                    }
                }

                if(!stacker.empty() ||  !utils::CharInString(ch_current, __SEPARATORS__))
                    value += content[i];
                else {
                    i--;
                }

                //значение прочитано полностью?
                if(stacker.empty()
                    && (utils::CharInString(ch_next, __SEPARATORS__ " ")
                        || ch_next == 0
                        || utils::CharInString(ch_current, __SEPARATORS__))
                    && (value.empty() || !utils::OnlySpaces(value))
                    )
                {
                    if(!ConfirmValue(true))
                        break; //если словили exception при обработке - выходим из цикла for()
                    //иначе отмечаем, что значение прочитано корректно
                    value_stored = true;
                }
            } else {
                // все остальные символы кроме пробелов - признак ошибки парсера
                if(!utils::CharInString(ch_current, __SPACES__)) {
                    CreateErrorUnexpected(counter, ch_current);
                    break;
                }
            }
        } // loop for()

        if(!error()) {
            result_cfg.setSuffixComment(utils::VStringToString(comments));
        } else {
            std::string error_msg = getError();
            result_cfg.setValue(); //значение не распознано, выставить в null
            result_cfg.setError(error_msg);
        }
        result_cfg.setPrefixComment(std::move(prefix_comment));
    } else {
        result_cfg = Config();
    }

    return !error();
}

bool Config::parseIni(const std::string &content, const CommentDesign &input_design) noexcept
{
    using namespace tools;

    setValue(Config(ValueType::eJson)); // clear() не нужен, т.к. объект только создан
    setCommentDesign(input_design);

    VString lines;
    // разбить content на строки
    for(size_t i = 0; i < content.size(); /*i++*/) {
        const size_t i_ = i;
        i = content.find('\n', i_);
        if(i == std::string::npos) {
            // std::cout << "new iter: [" << i_ << "," << (content.size() - 1) << "]" << std::endl;
            lines.push_back(content.substr(i_, content.size() - 1));
            break;
        } else {
            const size_t end_i = i > i_ ? i - 1 : i;
            // std::cout << "new iter: [" << i_ << "," << end_i << "]" << std::endl;
            lines.push_back(content.substr(i_, end_i - i_ + (i == i_ ? 0 : 1))); //уникальный случай "нулевая длина"
        }
        i += 1;
    }

    /* совместить многострочные значения:
     * - кавычки начаты, но не закончены  (скобки тоже считаются {}[]<>())  (+ \n + следующая, следующую удалить)
     * - есть обратный слэш в конце строки                                  (+ \n + следующая, следующую удалить)
     * - на следующей строке первыми символами идёт пробел/табуляция        (+ \n + следующая, следующую удалить)
     * - начат многострочный комментарий, но не закончен                    (+ \n + следующая, следующую удалить)
     */
    // NOTE: комментарии внутри значения являются частью значения, а не комментарием самого значения

    // обработка обратного слэша на конце строки и пробела в начале следующей
    VVString lines_vec;
    std::array<char, 3> current_comment_format = {0, 0, 0};
    while(!lines.empty()) {
        lines_vec.push_back({lines.front()});
        lines.erase(lines.cbegin());

        while(!lines.empty())
        {
            const char c = utils::GetLastNotSpaceChar(lines_vec.back().back());
            if(lines.size() >= 1) // следующая строка существует?
            {
                if(c == '\\' || utils::CharInString(lines.front().front(), "\t ")) //следующая строка является частью значения
                {
                    size_t erase_pos = lines_vec.back().back().find_last_of('\\');
                    if(erase_pos != std::string::npos) {
                        lines_vec.back().back().erase(erase_pos, lines_vec.back().back().size() - erase_pos);
                    }
                    lines_vec.back().back().push_back('\n'); //нужно для последующей конкатенации строк в одну большую

                    utils::RemoveFrontIllegalSpaces(lines.front());
                    lines_vec.back().push_back(lines.front());
                    lines.erase(lines.cbegin());
                }
                else break; //выход из while
            }
            else break; //выход из while
        }
    }
    // по выходу из цикла lines должна быть пуста

    //дополнить знаком переноса строки, если знака нет и строка не пустая
    for(auto& vstring : lines_vec) {
        for(auto& str : vstring) {
            if(!str.empty() && str.back() != '\n') {
                str.push_back('\n');
            }
        }
    }
    //последний перенос строки ломает счётчик символов
    if(!lines_vec.empty() && !lines_vec.back().empty() && !lines_vec.back().back().empty()) {
        lines_vec.back().back().pop_back();
    }

    auto CreateError = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError("ElementJson/ElementArray INI parser error[" + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorLine = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError("ElementJson/ElementArray INI parser error at line [" + std::to_string(counter.getLastLineCounter())
                 + "]: " + message + "!");
    };

    // один объект vlines - одно значение (часть многострочного комментария ПОСЛЕ значеня может остаться за бортом)
    Config* main_target = this; //точка привязки группы значений
    Config* target      = this; //точка привязки нового значения
    std::vector<std::string> prefix_comments;
    std::vector<std::string> suffix_comments;
    std::string current_comment;
    std::string temp_string_value;
    ParserSymbolCounter counter; // проинициализируется при обработке первой строки

    Stacker stacker;
    stacker.addSimpleRule('\'');
    stacker.addSimpleRule('"');
    stacker.addDoubleRule('[', ']');

    size_t assignment_counter = 0;
    char ch_previous          = 0;
    char ch_current           = 0;
    char ch_next              = 0;

    //отделить ключ от значения
    auto GetIniKey = [](std::string& content) -> std::string {
        std::string key;

        //находить знаки = и : до тех пор, пока не будут встречены лишние символы
        //ключ в кавычках не должен влиять на поиск
        Stacker stacker_lambda;
        stacker_lambda.addSimpleRule('\'');
        stacker_lambda.addSimpleRule('"');

        std::string temp;
        char ch_previous    = 0;
        char ch_current     = 0;
        size_t last_key_pos = 0;
        bool parsed         = false;
        for(size_t i = 0; i < content.size(); i++, ch_previous = ch_current) {
            ch_current  = content[i];
            stacker_lambda.autocheck(ch_current); // для проверки нахождения внутри кавычек

            if(!stacker_lambda.inQuotes()) {
                switch(ch_current) {
                    case ':':
                    case '=': {
                        utils::RemoveIllegalSpaces(temp);
                        key = temp;
                        temp.clear();
                        last_key_pos = i;
                        parsed = true;
                        break;
                    }
                    default: temp += ch_current;
                }
                if(parsed) break;
            }
        }
        if(!key.empty())
            content.erase(0, last_key_pos + 1);
        utils::RemoveIllegalSpaces(content);

        return key;
    };
    auto SplitIniKeyPath = [](const std::string& big_key) -> std::vector<std::string> {
        VString keys;
        Stacker inner_stacker;
        inner_stacker.addSimpleRule('\'');
        inner_stacker.addSimpleRule('"');

        //находить знаки / и \\ до тех пор, пока не будут встречены лишние символы
        //ключ в кавычках не должен влиять на поиск
        std::string temp;
        char ch_previous = 0;
        char ch_current = 0;
        size_t last_key_pos = 0;
        for(size_t i = 0; i < big_key.size(); i++, ch_previous = ch_current) {
            ch_current = big_key[i];

            if(ch_previous != '\\') {
                inner_stacker.autocheck(ch_current);
            }

            if(inner_stacker.empty()) {
                switch(ch_current) {
                case '/':
                case '\\': {
                    utils::RemoveIllegalSpaces(temp);
                    utils::RemoveQuotes(temp);
                    keys.push_back(temp);
                    temp.clear();
                    last_key_pos = i;
                    break;
                }
                default: temp += ch_current;
                }
            }
        }
        utils::RemoveIllegalSpaces(temp);
        utils::RemoveQuotes(temp);
        keys.push_back(temp);

        return keys;
    };
    auto ConfirmValue = [&]() -> void {
        utils::RemoveIllegalSpaces(temp_string_value);
        if(!temp_string_value.empty())
        {
            if(temp_string_value.front() == '[' && temp_string_value.back() == ']')
            {
                temp_string_value.erase(0, 1);
                temp_string_value.pop_back();

                //группы значений могут быть объявлены только для основного Config
                //NOTE: если группа с таким именем уже существует - дополнить её
                Config& cfg_main_target = GetFirstJsonFromThis(*main_target);
                cfg_main_target.push_back(temp_string_value, Config(ValueType::eJson));

                //создать либо дополнить префиксный комментарий
                if(cfg_main_target[temp_string_value].getPrefixComment().empty())
                    cfg_main_target[temp_string_value].setPrefixComment(utils::VStringToString(prefix_comments));
                else
                    cfg_main_target[temp_string_value].setPrefixComment(cfg_main_target[temp_string_value].getPrefixComment() + "\n"
                                                                        + utils::VStringToString(prefix_comments));
                //создать либо дополнить постфиксный комментарий
                if(cfg_main_target[temp_string_value].getSuffixComment().empty())
                    cfg_main_target[temp_string_value].setSuffixComment(utils::VStringToString(suffix_comments));
                else
                    cfg_main_target[temp_string_value].setSuffixComment(cfg_main_target[temp_string_value].getSuffixComment() + "\n"
                                                                        + utils::VStringToString(suffix_comments));

                target = &cfg_main_target[temp_string_value];
            }
            else if(!temp_string_value.empty())
            {
//                counter.printCoords(); //для отладки
                const std::string k = GetIniKey(temp_string_value);
                // если ключ многосотавной (вложенные структуры), то значение положить составное (k1->k2->k3=value)
                VString keys_path = SplitIniKeyPath(k);
                Config temp = CreateElementFromString(std::string(temp_string_value), ConfigFormat::eJSON, getCommentDesign(), counter);
                Config& pushed_cfg = GetFirstJsonFromThis(*target)
                                         .push_back_force(keys_path, std::move(temp));

                // добавить комментарии к добавленному значению
                pushed_cfg.setPrefixComment(utils::VStringToString(prefix_comments));
                pushed_cfg.setSuffixComment(utils::VStringToString(suffix_comments));
            }
            temp_string_value.clear();
            prefix_comments.clear();
            suffix_comments.clear();
            assignment_counter = 0;
        }
    };

    size_t k = 0; //вынес для видимости вне for()
    size_t all_lines_counter = 0;
    for(; k < lines_vec.size(); k++) {
        VString& fragments = lines_vec[k];

        bool need_to_iterate_k = false;

        counter = ParserSymbolCounter(all_lines_counter);
        for(size_t i = 0; i < fragments.size(); i++, all_lines_counter++) {
            if(getPrefixComment().empty() && !prefix_comments.empty() && fragments[i].empty()) {
                setPrefixComment(utils::VStringToString(prefix_comments));
                prefix_comments.clear();
            }

            for(size_t j = 0; j < fragments[i].size(); j++) {
                ch_previous  = ch_current; //будет 0, если вся предыдущая цепочка была прочитана как значение
                ch_current   = fragments[i][j];
                //если следующий символ в этой подстроке существует
                if(j + 1 < fragments[i].size()) {
                    ch_next = fragments[i][j + 1];
                } else {
                    //если следующая строка в этом фрагменте существует и не пустая
                    if(i + 1 < fragments.size() && !fragments[i + 1].empty()) {
                        ch_next = fragments[i + 1].front();
                    } else {
                        if(k + 1 < lines_vec.size()              // если следующий фрагмент существует
                            && !lines_vec[k + 1].empty()         // подстрока следующего фрагмента существует
                            && !lines_vec[k + 1].front().empty() // подстрока не пустая
                            )
                        {
                            ch_next = lines_vec[k + 1].front().front();
                        }
                    }
                }

                counter.check(j, ch_current);

                //поиск комментариев ===================================================
                //вернёт комментарий без обрамления
                try {
                    CheckComments(ch_current, ch_next, j, getCommentDesign(), current_comment, stacker.empty());
                } catch(std::exception& e) {
                    //не хватило символов для прочтения комментария
                    CreateError(counter, e.what());
                    break;
                }

                if(!getCommentDesign().with_comments)
                    current_comment.clear();
                if(getCommentDesign().with_comments && getCommentDesign().temp_type == CommentType::eCommentEnd)
                {
                    // для отладки
                    // std::cout << "comment (value_size:" << temp_string_value.size() << ")"
                    //           << (temp_string_value.empty() ? "(prefix)" : "(suffix)")
                    //           << ": \"" << FromComment(current_comment, design) << "\"" << std::endl;
                    if(temp_string_value.empty())
                        prefix_comments.push_back(FromComment(std::move(current_comment), getCommentDesign()));
                    else
                        suffix_comments.push_back(FromComment(std::move(current_comment), getCommentDesign()));
                    current_comment.clear();
                    getCommentDesign().temp_type = CommentType::eNotComment;
                    continue;
                }
                if(getCommentDesign().temp_type != CommentType::eNotComment)
                    continue;
                //=================================================== поиск комментариев

                if(temp_string_value.empty()
                    && stacker.empty()
                    && utils::CharInString(ch_current, __SPACES_WITHOUT_SEPARATORS__))
                {
                    continue;
                }

                if(ch_previous != '\\') {
                    if(!stacker.autocheck(ch_current)) { //проверяет только нахождение в кавычках
                        // недостижимый код ?
                        //error_string = "ERROR!";
                        break;
                    }
                }

                //здесь нет смысла проверять внешние условия - всё, что введёт пользователь - строка в том или ином виде
                temp_string_value += ch_current;

                if(utils::CharInString(ch_current, "=:")) {
                    assignment_counter++;
                }

                if(assignment_counter == 0 && ch_previous == '[' && ch_current == ']') {
                    CreateError(counter, "name of group must not be empty");
                    return false;
                }
            }
        }

        //значение корректно?
        if(!stacker.empty() || getCommentDesign().temp_type != CommentType::eNotComment)
        {
            continue; //значение прочитано не полностью!
        }

        ConfirmValue();
        ch_current  = 0; //обнуляем, чтобы следующий фрагмент рассматривался самостоятельно
    }

    //если входяшая строка закончилась раньше, чем было определено значение
    ConfirmValue();

    if(!prefix_comments.empty()) {
        setSuffixComment(utils::VStringToString(prefix_comments));
        prefix_comments.clear();
    }

    return !error();
}

bool Config::parseYaml(const std::string &content, const CommentDesign &design) noexcept
{
    //TODO: Config::parseYaml()
    return false;
}

bool Config::parseXml(const std::string &content, const CommentDesign &design) noexcept
{
    // NOTE: актуальный RFC https://www.rfc-editor.org/info/rfc7303/
    using namespace utils;
    using namespace tools;

    setValue(Config(ValueType::eXml)); // clear() не нужен, т.к. объект только создан
    setCommentDesign(design);

    std::vector<std::string> prefix_comments;
    std::vector<std::string> suffix_comments;
    VString comments; // обработанные комментарии
    std::string current_comment;
    std::string temp_string_value;
    ParserSymbolCounter counter; // проинициализируется при обработке первой строки

    const std::string error_template = "XML parser error";
    auto CreateError = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError("XML parser error[" + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorLine = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError("XML parser error at line [" + std::to_string(counter.getLastLineCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorUnexpected = [&](const ParserSymbolCounter& counter, char ch, std::string message = "") -> void {
        setError(error_template + ": unexpected symbol '" + ch + "' at ["
                 + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]" + (message.empty() ? "" : ": " + message + "!"));
    };

    auto ConfirmValue = [&, content](const bool for_penultimate = false) -> bool {
        // counter.printCoords(); // для отладки
//        Config element = CreateElementFromString(std::move(value), ConfigFormat::eJSON,
//                                                 static_cast<const Config&>(result_cfg).getCommentDesign(),
//                                                 start_value_counter);
//        if(element.error()) {
//            //если случилась ошибка при внутренней конвертации прочитанного значения,
//            // то эта ошибка становится основной ошибкой парсинга
//            CreateError(counter, element.getError());
            return false;
//        }

//        result_cfg = Config(std::move(element));
//        if(result_cfg.getCommentDesign().opt_multiline_column_size > n_design.opt_multiline_column_size)
//            n_design.opt_multiline_column_size = result_cfg.getCommentDesign().opt_multiline_column_size;

//        return true;
    };

    auto AppendMainPreviewComment = [&]() {
        if(!comments.empty() && (getCommentDesign().temp_type == CommentType::eCommentEnd
                                  || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            setPrefixComment(VStringToString(comments));
            DEBUG_LOG("ElementJson: PreviewComment: " << "\"" << getPrefixComment() << "\"");
            comments.clear();
        }
    };
    auto AppendMainSuffixComment = [&]() {
        if(!comments.empty() && (getCommentDesign().temp_type == CommentType::eCommentEnd
                                  || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            setSuffixComment(VStringToString(comments));
            DEBUG_LOG("ElementJson: SuffixComment: " << "\"" << getSuffixComment() << "\"");
            comments.clear();
        }
    };
// FIXME:    /* Логика работы комментариев:
// FIXME:     * - комментарий после значения применяется только при начале на той же строке,
// FIXME:     * что и разделитель этого значения
// FIXME:     * - все остальные комментарии добавляются перед следующим значением
// FIXME:     */
    auto AppendElementPrefixComment = [&](){
        if(!comments.empty()
            && !isEmpty()
            && (getCommentDesign().temp_type == CommentType::eCommentEnd
                || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            get_back().setPrefixComment(VStringToString(comments));
            DEBUG_LOG("ElementJson: inner Element add PreviewComment: " << "\"" << get_back().getPrefixComment() << "\"");
            comments.clear();
        }
    };
    auto AppendElementSuffixComment = [&](const bool for_penultimate = false){
        if(!comments.empty()
            && !isEmpty()
            && (getCommentDesign().temp_type == CommentType::eCommentEnd
                || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            if(for_penultimate) {
                if(get_at(size() - 2).getSuffixComment().empty())
                {
                    get_at(size() - 2).setSuffixComment(comments[0]);
                    DEBUG_LOG("ElementJson: inner Element(penultimate) add SuffixComment: " << "\""
                              << get_at(size() - 2).getSuffixComment() << "\"");
                }
                comments.erase(comments.cbegin());
            } else {
                if(get_back().getSuffixComment().empty())
                {
                    get_back().setSuffixComment(comments.back());
                    DEBUG_LOG("ElementJson: inner Element(back) add SuffixComment: " << "\""
                              << get_back().getSuffixComment() << "\"");
                }
                comments.pop_back();
            }

        }
    };


    Stacker stacker;
    stacker.addDoubleRule('<', '>');

    char ch_previous = 0;
    char ch_current  = 0;
    char ch_next     = 0;

    /* Логика парсера:
     * - есть открывающий ТЕГ <X>
     * - есть закрывающий ТЕГ </X>
     * - если ТЕГ не имеет ВЛОЖЕННОГО СОДЕРЖИМОГО, тогда открывающий ТЕГ является закрывающим <X/>
     * - комментарии имеют свой протокольный синтаксис <!-- COMMENT -->
     *   - SimpleAPI считывает ЛЮБЫЕ комментарии согласно протоколу XML и правилам CommentDesign (p.s. запись только по протоколу)
     * - тег может содержать АТРИБУТЫ
     * - на верхнем уровне может быть максимум один ТЕГ
     * - перед ТЕГом верхнего уровня могут быть только ПРОЛОГ и КОММЕНТАРИИ
     * - после ТЕГа верхнего уровня могут быть только ЭПИЛОГ и КОММЕНТАРИИ
     */
    std::string current_tag, value;
    VString attributes;
    std::string temp;
    bool b_tag_closer_is_found = false;
    ParseStateXml state = ParseStateXml::eXML_FORMAT;
    for(size_t i = 0; i < content.size(); i++)
    {
        ch_previous = i == 0 ? 0 : content[i - 1];
        ch_current  = content[i];
        ch_next     = i < content.size() ? content[i + 1] : 0;

        counter.check(i, ch_current);

        /*
        //поиск комментариев ===================================================
        // TODO: нужно также учитывать стиль этого протокола: <!-- COMMENT -->
        //вернёт комментарий без обрамления
//        try {
//            CheckComments(ch_current, ch_next, i, getCommentDesign(), current_comment, stacker.empty());
//        } catch(std::exception& e) {
//            //не хватило символов для прочтения комментария
//            CreateError(counter, e.what());
//            break;
//        }

//        if(!getCommentDesign().with_comments)
//            current_comment.clear();
//        if(getCommentDesign().with_comments && getCommentDesign().temp_type == CommentType::eCommentEnd)
//        {
//            comments.push_back(FromComment(std::move(current_comment), getCommentDesign()));
//            current_comment.clear();
//            getCommentDesign().temp_type = CommentType::eNotComment;
//            continue;
//        }
//        if(getCommentDesign().temp_type != CommentType::eNotComment)
//            continue;
        //=================================================== поиск комментариев
        */

        // найти символ вхождения <
        // прочитать ВЕСЬ ТЕГ до символа выхода >
        // определить тип ОТКРЫВАЮЩИЙ или САМОЗАКРЫВАЮЩИЙСЯ по последнему символу содержимого

        if(!stacker.autocheck(ch_current))
        {
            CreateErrorUnexpected(counter, ch_current);
            break;
        }

        // ищем вхождение в описание тега
        // считываем имя тега (наличие пробелов перед именем - критичная ошибка (оригинальных парсеров))
        // если встретили пробел ПОСЛЕ ИМЕНИ ТЕГА - ожидаем атрибуты или закрывающую треугольную скобку
        // атрибуты всегда по маске КЛЮЧ="ЗНАЧЕНИЕ", кавычки обязательны для оригинальных парсеров
        // если тег не самозакрывающий, то всё до маски </ИМЯ_ТЕГА[:SPACES:]*> считаем внутренним значением и передаём другому парсеру

        switch(state) {
        case ParseStateXml::eXML_FORMAT: {
            // раздел необязательный
            // если формат отличается от utf-8 - вернуть как ошибку парсинга

            // пробелы и пустые символы ничего не значат - пропускаем
            if(temp_string_value.empty() && utils::CharInString(ch_current, __SPACES__))
                continue;

            if(temp_string_value.empty())
            {
                if(ch_current == '<') {
                    temp_string_value += ch_current;

                    switch(ch_next) {
                    case '?': {
                        // это действительно строка определения формата
                        break;
                    }
                    case '!': {
                        UpdateState(state, ParseStateXml::eXML_PROLOGUE);
                        break;
                    }
                    default: {
                        UpdateState(state, ParseStateXml::eXML_TAG_START);
                        break;
                    }
                    }
                } else {
                    CreateErrorUnexpected(counter, ch_current);
                }
            } else {
                // значение уже начало заполняться
                temp_string_value += ch_current;

                // маска проверки: <?xml X="Y" Z="A" ?>

                if(ch_current == '>') {
                    // закончили распознавать строку формата, меняем состояние на ожидание пролога
                    // NOTE: строка формата всегда одна и всегда в начале документа

                    std::cout << "format string: \"" << temp_string_value << "\"" << std::endl;
                    /* TODO: валидация внутренних параметров */ {
                        temp_string_value.clear();
                    }

                    UpdateState(state, ParseStateXml::eXML_PROLOGUE);
                }
            }

            break; // выход из switch
        }
        case ParseStateXml::eXML_PROLOGUE: {
            // раздел необязательный
            // здесь могут быть настройки валидации полей и т.п.

            // пробелы и пустые символы ничего не значат - пропускаем
            if(temp_string_value.empty() && utils::CharInString(ch_current, __SPACES__))
                continue;

            if(temp_string_value.empty())
            {
                if(ch_current == '<') {
                    temp_string_value += ch_current;

                    switch(ch_next) {
                    case '?': {
                        auto temp_counter = counter;
                        temp_counter.check(i+1, ch_next);
                        CreateErrorUnexpected(counter, ch_next, "unexpected instruction of XML");
                        break;
                    }
                    case '!': {
                        // это действительно строка описания типов и структур
                        break;
                    }
                    default: {
                        UpdateState(state, ParseStateXml::eXML_TAG_START);
                        break;
                    }
                    }
                } else {
                    CreateErrorUnexpected(counter, ch_current);
                }
            } else {
                // значение уже начало заполняться
                temp_string_value += ch_current;

                // маска проверки: <!TYPE_OF_VALUE X="Y" Z="A" >

                if(ch_current == '>') {
                    // закончили распознавать строку описания
                    // состояние не меняем - пролог может содержать сколько угодно описаний

                    std::cout << "prolog string: \"" << temp_string_value << "\"" << std::endl;
                    /* TODO: валидация внутренних параметров */ {
                        temp_string_value.clear();
                    }
                }
            }

            break; // выход из switch
        }
        case ParseStateXml::eXML_TAG_START: {
            // запрещены пробелы между < и именем тега (игнорировать в рамках этого парсера)
            // первым символом имени тега может быть только буква или знак подчёркивания '_'

            // пробелы и пустые символы ничего не значат - пропускаем
            if(utils::CharInString(ch_current, __SPACES__)) {
                if(temp_string_value.empty()) {
                    continue; // дожидаемся значения
                } else {
                    // тег прочитан запоминаем
                    current_tag = temp_string_value;
                    if(current_tag.front() == '<')
                        current_tag.erase(0, 1); // удаляем обрамление

//                    std::cout << "tag: \"" << temp_string_value << "\"" << std::endl;
                    temp_string_value.clear();

                    // был встречен пробел после тега - ожидаем атрибуты
                    UpdateState(state, ParseStateXml::eXML_ATTRIBUTES);
                    // только в рамках атрибутов работает проверка на кавычки
                    stacker.addSimpleRule('\'');
                    stacker.addSimpleRule('"');

                    break; // выход из switch
                }
            }

            // выдержка из ИИ Google:
            // 1) Запрет на «xml»: Имя тега не может начинаться с букв xml, XML, Xml и любых других комбинаций регистров.
            //    Это сочетание зарезервировано.
            // 2) Первый символ: Имя может начинаться только с буквы (любого алфавита, включая кириллицу) или знака
            //    подчеркивания _.
            // 3) Запрет на цифры в начале: Имя не может начинаться с цифры, точки или дефиса.
            // 4) Разрешенные символы внутри: После первого символа можно использовать буквы, цифры, дефисы -, знаки
            //    подчеркивания _ и точки ..
            // 5) Запрет на пробелы: Внутри имени тега нельзя ставить пробелы.
            // 6) Проблема с двоеточием: Символ : технически разрешен, но зарезервирован для пространств имен (namespaces).
            //    Использовать его просто так в именах тегов нельзя.

            // если после пробела есть сочетание "/>", тогда тег считается самозокрытым - внутреннего значения нет
            // если после пробела идёт знак >, тогда атрибутов нет
            //   в остальном случае нужно перейти к парсингу атрибутов

            if(temp_string_value.empty())
            {
                if(ch_current == '<') {
//                    temp_string_value += ch_current;

                    switch(ch_next) {
                    case '?': {
                        auto temp_counter = counter;
                        temp_counter.check(i+1, ch_next);
                        CreateErrorUnexpected(counter, ch_next, "unexpected instruction of XML");
                        break;
                    }
                    case '!': {
                        auto temp_counter = counter;
                        temp_counter.check(i+1, ch_next);
                        CreateErrorUnexpected(counter, ch_next, "unexpected instruction of XML");
                        break;
                    }
                    default: {
                        // начинаем чтение имени тега
                        break;
                    }
                    }
                } else {
                    CreateErrorUnexpected(counter, ch_current);
                }
            } else {
                // TODO: валидация добавляемого имени
                {}

                // маска проверки: <NAME_OF_TAG >
                // атрибуты будут прочтены отдельно при наличии пробела после имени

                if(utils::CharInString(ch_current, "/>")) {
                    // две ситуации: либо "/>", либо ">"
                    if(ch_current == '/') {
//                        current_tag += "/>";
                        // тег самозакрытый, внутреннего значения не будет; на верхнем уровне может быть только один тег
                        UpdateState(state, ParseStateXml::eXML_EPILOGUE);
                    } else {
//                        current_tag += ">";
                        UpdateState(state, ParseStateXml::eXML_INNER_VALUE);
                    }

                    // закончили распознавать строку описания
                    // состояние не меняем - пролог может содержать сколько угодно описаний

//                    std::cout << "tag name: \"" << temp_string_value << "\"" << std::endl;
                    temp_string_value.clear();
                } else {
                    // значение уже начало заполняться
                    temp_string_value += ch_current;
                }
            }

            break; // выход из switch
        }
        case ParseStateXml::eXML_ATTRIBUTES: {
            // всегда валидировать по маске ключ=значение, где значение обязательно в кавычках (одинарных или двойных)

            // пока кавычки - только запоминаем значение
            if(stacker.inQuotes()) {
                temp_string_value += ch_current;
                break; // выход из switch
            }

            // пробелы и пустые символы ничего не значат - пропускаем
            if(utils::CharInString(ch_current, __SPACES__)) {
                if(temp_string_value.empty()) {
                    continue; // дожидаемся значения
                } else {
                    // атрибут прочитан запоминаем
                    if(!temp_string_value.empty())
                    {
                        attributes.push_back(temp_string_value);
                        temp_string_value.clear();

//                        std::cout << "attribute: \"" << attributes.back() << "\"" << std::endl;
                    }

                    break; // выход из switch
                }
            }

            // (то же поведение, что и без атрибутов) // TODO: может добавить state eXML_TAG_START_ENDING
            // если после пробела есть сочетание "/>", тогда тег считается самозокрытым - внутреннего значения нет
            // если после пробела идёт знак >, тогда атрибутов нет

            if(utils::CharInString(ch_current, "/>"))
            {
                // две ситуации: либо "/>", либо ">"
                if(ch_current == '/') {
//                    current_tag += "/>";
                    // тег самозакрытый, внутреннего значения не будет; на верхнем уровне может быть только один тег
                    UpdateState(state, ParseStateXml::eXML_EPILOGUE);
                } else {
//                    current_tag += ">";
                    UpdateState(state, ParseStateXml::eXML_INNER_VALUE);
                }

                // атрибут прочитан запоминаем
                if(!temp_string_value.empty())
                {
                    attributes.push_back(temp_string_value);
                    temp_string_value.clear();

//                    std::cout << "attribute: \"" << attributes.back() << "\"" << std::endl;
                }

                // удаляем правила проверки кавычек
                stacker.deleteSimpleRule('"');
                stacker.deleteSimpleRule('\'');

                break; // выход из switch
            } else {
                // TODO: валидация добавляемого имени
                {}

                // значение уже начало заполняться
                temp_string_value += ch_current;

                // маска проверки: <NAME_OF_TAG >
                // атрибуты будут прочтены отдельно при наличии пробела после имени
            }

            break; // выход из switch
        }
        case ParseStateXml::eXML_INNER_VALUE: {
            // раздел необязательный
            // может содержать несколько значений, как XML-элементов, так и обычных строк
            //    для строк кавычки не валидируются

            // пробелы и пустые символы ничего не значат - пропускаем
            if(temp_string_value.empty() && utils::CharInString(ch_current, __SPACES__))
                continue;

            // добавляем значение, пока не встретили закрывающий тег
            temp_string_value += ch_current;
            if(ch_current == '<' && ch_next == '/') {
                b_tag_closer_is_found = true;
            }
            // нашли закрывающую скобку
            if(b_tag_closer_is_found && ch_current == '>') {
                b_tag_closer_is_found = false; // сбрасываем флаг

                // проверяем, что тег соответствует искомому
                size_t start_pos = temp_string_value.rfind("</");
                // на std::string::npos не проверяю в целях экономии (мы ж именно по этому флагу сюда зашли)
                std::string temp_tag_closer = temp_string_value.substr(start_pos);

                temp_tag_closer.erase(0, 2);
                temp_tag_closer.pop_back();
                std::cout << "closer: " << temp_tag_closer << std::endl;
                if(temp_tag_closer == current_tag) {
                    value = temp_string_value.erase(start_pos);
                    temp_string_value.clear();

                    // TODO: value нужно передать на следующий парсер
                    {}

                    UpdateState(state, ParseStateXml::eXML_EPILOGUE);
                }
            }

            break; // выход из switch
        }
        // этот case пропускаю, т.к. можно облегчить логику поиска за счёт уже прочитанного значения
//        case ParseStateXml::eXML_TAG_FINISH: {
//            // XML-элемент считается закрытым, если есть совпадение маски "</[:SPACES:]TAG_NAME*[:SPACES:]*>"
//            //    пробелы перед именем тега оригинальным парсером считаются критической ошибкой
//            break; // выход из switch
//        }
        case ParseStateXml::eXML_EPILOGUE: {
            // раздел необязательный
            break; // выход из switch
        }
        case ParseStateXml::eXML_ERROR: {
            break; // выход из switch
        }
        default: break; // выход из switch
        } // ~switch()

        if(error())
            break; // выход из for()
    } // ~for()

    std::cout << "tag name: " << current_tag << std::endl;
    for(const auto& attribute : attributes)
        std::cout << "attribute: " << attribute << std::endl;
    std::cout << "value: " << value << std::endl;
    std::cout << "last state: " << ToString(state) << std::endl;

    // наличие ошибки строго обнуляет структуру документа
    if(error())
        clear();

    return !error();
}

std::string Config::ToString(const ParseStateJson state) noexcept {
    switch (state) {
        case ParseStateJson::eJSON_START:               return "[JSON_START]";
        case ParseStateJson::eJSON_KEY:                 return "[JSON_KEY]";
        case ParseStateJson::eJSON_KEY_VALUE_SEPARATOR: return "[JSON_KEY_VALUE_SEPARATOR]";
        case ParseStateJson::eJSON_VALUE:               return "[JSON_VALUE]";
        case ParseStateJson::eJSON_SEPARATOR:           return "[JSON_SEPARATOR]";
        case ParseStateJson::eJSON_FINISH:              return "[JSON_FINISH]";
        case ParseStateJson::eJSON_ERROR_STATE:
        default:                                        return "[JSON_ERROR_STATE]";
    }
}

void Config::UpdateState(ParseStateJson &state, const ParseStateJson new_state) noexcept {
    state = new_state;
    DEBUG_LOG("Parse Json, upd state: " << ToString(state));
}

void Config::parseFullJsonDoc(std::string &&content) noexcept
{
    using namespace utils;
    using namespace tools;

    /* NOTE: структура Json (для документации)
     * комментарий json
     * начало json
     * (+комментарий перед ключом)
     * значение элемента json
     * (+комментарий после значения)    = на строке значения
     * (разделитель)
     * (+комментарий перед ключом)      = после разделителя
     * (+значение элемента json)        = после разделителя
     * (+комментарий после значения)    = после разделителя
     * конец json
    */

    //пустой документ не является ошибкой синтаксиса
    RemoveIllegalSpaces(content);
    if(content.empty()) return;

    getCommentDesign().temp_type    = CommentType::eNotComment;

    ParseStateJson state            = ParseStateJson::eJSON_START;
    std::string key                 = "";
    std::string value               = "";

    Stacker stacker;
    stacker.addSimpleRule('\'');
    stacker.addSimpleRule('"');
    stacker.addDoubleRule('[', ']');
    stacker.addDoubleRule('{', '}');

    char last_separator_symbol      = '\n';
    bool is_one_value_format        = false; //одиночные значения не требуют фигурных скобок
    ParserSymbolCounter counter;
    ParserSymbolCounter start_value_counter; //для счётчика внутри значения

    std::string current_comment     = ""; // текущее значение при парсинге
    VString comments;                     // обработанные комментарии
    ssize_t value_read_at_line       = -1;

    const std::string error_template = "ElementJson JSON parser error";
    auto CreateError = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError(error_template + "[" + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorLine = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError(error_template + " at line [" + std::to_string(counter.getLastLineCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorUnexpected = [&](const ParserSymbolCounter& counter, char ch, std::string message = "") -> void {
        setError(error_template + ": unexpected symbol '" + ch + "' at ["
                 + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]" + (message.empty() ? "" : ": " + message + "!"));
    };

    auto AppendMainPreviewComment = [&]() {
        if(!comments.empty() && (getCommentDesign().temp_type == CommentType::eCommentEnd
                                  || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            setPrefixComment(VStringToString(comments));
            DEBUG_LOG("ElementJson: PreviewComment: " << "\"" << getPrefixComment() << "\"");
            comments.clear();
        }
    };
    auto AppendMainSuffixComment = [&]() {
        if(!comments.empty() && (getCommentDesign().temp_type == CommentType::eCommentEnd
                                  || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            setSuffixComment(VStringToString(comments));
            DEBUG_LOG("ElementJson: SuffixComment: " << "\"" << getSuffixComment() << "\"");
            comments.clear();
        }
    };

    /* Логика работы комментариев:
     * - комментарий после значения применяется только при начале на той же строке,
     * что и разделитель этого значения
     * - все остальные комментарии добавляются перед следующим значением
     */
    auto AppendElementPrefixComment = [&](){
        if(!comments.empty()
            && !isEmpty()
            && (getCommentDesign().temp_type == CommentType::eCommentEnd
                || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            get_back().setPrefixComment(VStringToString(comments));
            DEBUG_LOG("ElementJson: inner Element add PreviewComment: " << "\"" << get_back().getPrefixComment() << "\"");
            comments.clear();
        }
    };
    auto AppendElementSuffixComment = [&](const bool for_penultimate = false){
        if(!comments.empty()
            && !isEmpty()
            && (getCommentDesign().temp_type == CommentType::eCommentEnd
                || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            if(for_penultimate) {
                if(get_at(size() - 2).getSuffixComment().empty())
                {
                    get_at(size() - 2).setSuffixComment(comments[0]);
                    DEBUG_LOG("ElementJson: inner Element(penultimate) add SuffixComment: " << "\""
                              << get_at(size() - 2).getSuffixComment() << "\"");
                }
                comments.erase(comments.cbegin());
            } else {
                if(get_back().getSuffixComment().empty())
                {
                    get_back().setSuffixComment(comments.back());
                    DEBUG_LOG("ElementJson: inner Element(back) add SuffixComment: " << "\""
                              << get_back().getSuffixComment() << "\"");
                }
                comments.pop_back();
            }

        }
    };

    //дублирующийся код чтения значения
    auto ConfirmValue = [&, content](const bool for_penultimate = false) -> bool {
        DEBUG_LOG("ElementJson: current value done: \"" << value << "\"");
//        counter.printCoords(); // для отладки

        Config element = CreateElementFromString(std::move(value), ConfigFormat::eJSON, getCommentDesign(), start_value_counter);
        if(element.error()) {
            //контейнеры имеют добавляются если успешно прочитано хотя бы одно значение
            if(element.isArray() && !element.isEmpty()) {
                push_back(std::move(key), std::move(element));
                if(get_back().getCommentDesign().opt_multiline_column_size > getCommentDesign().opt_multiline_column_size)
                    getCommentDesign().opt_multiline_column_size = get_back().getCommentDesign().opt_multiline_column_size;
            }

            //если случилась ошибка при внутренней конвертации прочитанного значения,
            // то эта ошибка становится основной ошибкой парсинга
            CreateError(counter, "value parse error: " + element.getError());
            UpdateState(state, ParseStateJson::eJSON_ERROR_STATE);
            return false;
        }

        push_back(std::move(key), std::move(element));
        if(get_back().getCommentDesign().opt_multiline_column_size > getCommentDesign().opt_multiline_column_size)
            getCommentDesign().opt_multiline_column_size = get_back().getCommentDesign().opt_multiline_column_size;
        key.clear();
        value.clear();

        return true;
    };

    for(size_t i = 0; i < content.size() && state != ParseStateJson::eJSON_ERROR_STATE; i++) {
        char ch_previous = i == 0 ? 0 : content[i - 1];
        char ch_current  = content[i];
        char ch_next     = i < content.size() ? content[i + 1] : 0;

        counter.check(i, ch_current);

        //поиск комментариев ===================================================
        //вернёт комментарий без обрамления
        try {
            CheckComments(ch_current, ch_next, i, getCommentDesign(), current_comment, stacker.empty());
        } catch(std::exception& e) {
            //не хватило символов для прочтения комментария
            CreateError(counter, e.what());
            break;
        }

        if(!getCommentDesign().with_comments)
            current_comment.clear();
        if(getCommentDesign().with_comments && getCommentDesign().temp_type == CommentType::eCommentEnd)
        {
            comments.push_back(FromComment(std::move(current_comment), getCommentDesign()));
            current_comment.clear();
            getCommentDesign().temp_type = CommentType::eNotComment;
            continue;
        }
        if(getCommentDesign().temp_type != CommentType::eNotComment)
            continue;
        //=================================================== поиск комментариев

        // обработка комментария ПОСЛЕ значения
        if(utils::CharInString(ch_current, __SEPARATORS__)) {
            if(state == ParseStateJson::eJSON_KEY
                && key.empty()
                && !comments.empty()
                && (last_separator_symbol == ','
                    || (ch_current == '\n' && last_separator_symbol == ',' && comments[0].find('\n') == std::string::npos)))
            {
                //для отладки
                // std::cout << "last_separator_symbol: '"
                //           << (last_separator_symbol == '\n' ? "\\n" : std::string(&last_separator_symbol, 1))
                //           << "'" << std::endl;
                AppendElementSuffixComment();
            }
            /* таким образом запоминаются последние два разделителя:
             * - last_separator_symbol  - предыдущий
             * - ch_current             - текущий
             */
            last_separator_symbol = ch_current;
        }

        switch (state) {
        case ParseStateJson::eJSON_START: {
            //пропуск пробелов =====================================================
            if(CharInString(ch_current, __SPACES__)) break;
            //===================================================== пропуск пробелов

            if(ch_current == '{') {
                // работа с комментариями (до разбора json)
                AppendMainPreviewComment();
            } else {
                --i; //этот же символ уже является частью ключа
                is_one_value_format = true;
            }

            UpdateState(state, ParseStateJson::eJSON_KEY);
            break;
        }
        case ParseStateJson::eJSON_KEY: {
            //игнор пробелов и разделителей пока ключ пустой
            if(stacker.empty()
                && key.empty()
                && CharInString(ch_current, __SPACES__))
            {
                break;
            }
            if(stacker.empty() && ch_current == '}') {
                // работа с комментарием после элемента
                if(!isEmpty() && get_back().getSuffixComment().empty())
                    AppendElementSuffixComment();

                UpdateState(state, ParseStateJson::eJSON_FINISH);
                break;
            }

            if(ch_previous != '\\') {
                if(!stacker.autocheck(ch_current)) {
                    CreateErrorUnexpected(counter, ch_current);
                    UpdateState(state, ParseStateJson::eJSON_ERROR_STATE);
                    break;
                }
            }

            if(!stacker.empty() || !CharInString(ch_current, ":="))
                key += ch_current;
            else {
                i--;
            }

            //ключ прочитан полностью?
            if(stacker.empty() && (CharInString(ch_next, __SPACES__ ":=")
                                    || CharInString(ch_current, __SPACES__ ":=")) //т.к. пустой ключ - не ошибка
                )
            {
                RemoveIllegalSpaces(key); //игнорировать незначащие пробелы
                RemoveQuotes(key);
                DEBUG_LOG("ElementJson: current key done: \"" << key << "\"");
                UpdateState(state, ParseStateJson::eJSON_KEY_VALUE_SEPARATOR);
                value.clear();
            }

            break;
        }
        case ParseStateJson::eJSON_KEY_VALUE_SEPARATOR: {
            if(CharInString(ch_current, __SPACES__))
                break;
            if(CharInString(ch_current, ":=")) {
                UpdateState(state, ParseStateJson::eJSON_VALUE);

                //следующего символа не существует -> значением является null
                if(ch_next == 0) {
                    ConfirmValue(); //для предпоследнего элемента заполнить замыкающий комментарий

                    // проверка замыкающего комментария (вторичная)
                    if(value_read_at_line == counter.getLastLineCounter())
                        AppendElementSuffixComment(true);
                }

                break;
            }

            CreateError(counter, "not found json key-value separator ':' or '='");
            UpdateState(state, ParseStateJson::eJSON_ERROR_STATE);
            break;
        }
        case ParseStateJson::eJSON_VALUE: {
            //игнор пробелов пока значение пустое
            if(stacker.empty()
                && value.empty()
                && CharInString(ch_current, __SPACES__))
            {
                break;
            }

            if(value.empty())
                start_value_counter = counter; //будет использовано в CreateElementFromString()

            bool skip_append = false;
            if(ch_previous != '\\') {
                if(stacker.empty() && ch_current == '}')
                {
                    //уникальный случай - пустое значение перед завершением чтения значения
                    skip_append = true;
                    i--;
                } else if (!stacker.autocheck(ch_current))
                {
                    CreateErrorUnexpected(counter, ch_current);
                    UpdateState(state, ParseStateJson::eJSON_ERROR_STATE);
                    break;
                }
            }

            if(!skip_append) {
                if(!stacker.empty() || !CharInString(ch_current, __SEPARATORS__))
                    value += ch_current;
                else {
                    i--;
                }
            }

            //значение прочитано полностью?
            if(stacker.empty()
                && (CharInString(ch_next, __SEPARATORS__ " }")
                    || ch_next == 0
                    || CharInString(ch_current, __SEPARATORS__)
                    || CharInString(ch_current, __SEPARATORS__ " }") //т.к. пустой ключ - не ошибка
                    )
                && (value.empty() || !utils::OnlySpaces(value))
                )
            {
                if(!ConfirmValue(true))
                    break; //если словили exception при обработке - выходим из цикла for()

                // проверка замыкающего комментария (вторичная)
                if(value_read_at_line == counter.getLastLineCounter())
                    AppendElementSuffixComment(true);

                // работа с комментариями перед элементом
                AppendElementPrefixComment();

                UpdateState(state, ParseStateJson::eJSON_SEPARATOR);
            }
            break;
        }
        case ParseStateJson::eJSON_SEPARATOR: {
            //пропуск пробелов =====================================================
            if(CharInString(ch_current, __SPACES_WITHOUT_SEPARATORS__)) break;
            //===================================================== пропуск пробелов

            //может встретиться разделитель или знак завершения массива
            //  комментарий ПОСЛЕ значения может начинаться только на той же строке, что и значение
            //  разделитель может быть как ДО, так и ПОСЛЕ комментария
            //  если комментарий расписан после переноса строки, но до знака }, то комментарий попадёт в суффикс основы

            // запоминаем номер строки, на котором закончили считывать значение
            value_read_at_line = counter.getLastLineCounter(); //применится перед } и перед считыванием значения

            if(!is_one_value_format && CharInString(ch_current, __SEPARATORS__)) {
                UpdateState(state, ParseStateJson::eJSON_KEY);
                break;
            }
            if(!is_one_value_format && CharInString(ch_current, "}")) {
                UpdateState(state, ParseStateJson::eJSON_FINISH);
                break;
            }

            //либо считано одно значение и всё следующее является ошибкой (комменты не учитываются)
            //либо считано несколько значений и не найден знак завершения (комменты не учитываются)
            CreateError(counter, "not found end of JSON structure or value separator");
            UpdateState(state, ParseStateJson::eJSON_ERROR_STATE);
            break;
        }
        case ParseStateJson::eJSON_FINISH: {
            CreateErrorUnexpected(counter, ch_current, "after end of JSON structure there can only be comments");
            break;
        }
        default: break;
        }
    }

    /*если файл закончился раньше, чем было обработано последнее прочитанное значение*/
    if(!value.empty()
        && state == ParseStateJson::eJSON_VALUE)
    {
        if(getCommentDesign().temp_type == CommentType::eOneLineComment)
            getCommentDesign().temp_type = CommentType::eNotComment; //сбрасываем для корректной обработки oneline comment

        ConfirmValue();

        // проверка замыкающего комментария (вторичная)
        AppendElementSuffixComment();

        // работа с комментариями перед элементом
        AppendElementPrefixComment();
    }

    if(!is_one_value_format) {
        AppendMainSuffixComment(); //конечный комментарий для всего Json
    }

    if(!is_one_value_format && state != ParseStateJson::eJSON_FINISH)
    {
        if(!error())
            setError(error_template + "not found end of JSON structure or value separator"); //если встретили конец файла
        DEBUG_LOG("ERROR: " << getError());
        //NOTE: в случае ошибки парсинга корректно прочитанные значения сохраняются
        //clear();
    }
}

std::string Config::ToString(const ParseStateJsonArray state) noexcept {
    switch (state) {
        case ParseStateJsonArray::eARRAY_START:         return "[ARRAY_START]";
        case ParseStateJsonArray::eARRAY_VALUE:         return "[ARRAY_VALUE]";
        case ParseStateJsonArray::eARRAY_SEPARATOR:     return "[ARRAY_SEPARATOR]";
        case ParseStateJsonArray::eARRAY_FINISH:        return "[ARRAY_FINISH]";
        case ParseStateJsonArray::eARRAY_ERROR_STATE:
        default:                                        return "[ARRAY_ERROR_STATE]";
    }
}

void Config::UpdateState(ParseStateJsonArray& state, const ParseStateJsonArray new_state) noexcept {
    state = new_state;
    DEBUG_LOG("Parse Array, upd state: " << ToString(state) << std::endl);
}

void Config::parseFullJsonArrayDoc(std::string&& content) noexcept {
    using namespace utils;
    using namespace tools;

    /* NOTE: для документации
     * комментарий массива
     * начало массива (формат JSON)
     * (+комментарий перед значением)
     * значение массива
     * (+комментарий после значения) = на строке значения
     * (разделитель)
     * (+комментарий перед значением)= после разделителя
     * (+значение массива)           = после разделителя
     * (+комментарий после значения) = после разделителя
     * конец массива
    */

    //пустой документ не является ошибкой синтаксиса
    RemoveIllegalSpaces(content);
    if(content.empty()) return;

    getCommentDesign().temp_type = CommentType::eNotComment;

    ParseStateJsonArray state       = ParseStateJsonArray::eARRAY_START;
    std::string key                 = "";
    std::string value               = "";

    Stacker stacker;
    stacker.addSimpleRule('\'');
    stacker.addSimpleRule('"');
    stacker.addDoubleRule('[', ']');
    stacker.addDoubleRule('{', '}');

    char last_separator_symbol      = '\n';

    ParserSymbolCounter counter;
    ParserSymbolCounter start_value_counter; //для счётчика внутри значения

    std::string current_comment     = ""; // текущее значение при парсинге
    VString comments;                     // обработанные комментарии
    size_t value_read_at_line       = 0;

    const std::string error_template = "ElementArray JSON parser error";
    auto CreateError = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError(error_template + "[" + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorLine = [&](const ParserSymbolCounter& counter, std::string message) -> void {
        setError(error_template + " at line [" + std::to_string(counter.getLastLineCounter())
                 + "]: " + message + "!");
    };
    auto CreateErrorUnexpected = [&](const ParserSymbolCounter& counter, char ch, std::string message = "") -> void {
        setError(error_template + ": unexpected symbol '" + ch + "' at ["
                 + std::to_string(counter.getLastLineCounter())
                 + "][" + std::to_string(counter.getLastSymbolCounter())
                 + "]" + (message.empty() ? "" : ": " + message + "!"));
    };

    auto AppendMainPreviewComment = [&]() {
        if(!comments.empty() && (getCommentDesign().temp_type == CommentType::eCommentEnd
                                  || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            setPrefixComment(VStringToString(comments));
            DEBUG_LOG("ElementArray: PreviewComment: " << "\"" << getPrefixComment() << "\"");
            comments.clear();
        }
    };
    auto AppendMainSuffixComment = [&]() {
        if(!comments.empty() && (getCommentDesign().temp_type == CommentType::eCommentEnd
                                  || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            setSuffixComment(VStringToString(comments));
            DEBUG_LOG("ElementArray: SuffixComment: " << "\"" << getSuffixComment() << "\"");
            comments.clear();
        }
    };

    /* Логика работы комментариев:
     * - комментарий после значения применяется только при начале на той же строке,
     * что и разделитель этого значения
     * - все остальные комментарии добавляются перед следующим значением
     */
    auto AppendElementPrefixComment = [&](){
        if(!comments.empty()
            && !isEmpty()
            && (getCommentDesign().temp_type == CommentType::eCommentEnd
                || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            get_back().setPrefixComment(VStringToString(comments));
            DEBUG_LOG("ElementArray: inner Element add PreviewComment: " << "\"" << get_back().getPrefixComment() << "\"");
            comments.clear();
        }
    };
    auto AppendElementSuffixComment = [&](const bool for_penultimate = false){
        if(!comments.empty()
            && size() > (for_penultimate ? 2 : 1)
            && (getCommentDesign().temp_type == CommentType::eCommentEnd
                || getCommentDesign().temp_type == CommentType::eNotComment))
        {
            if(for_penultimate) {
                if(get_at(size() - 2).getSuffixComment().empty())
                {
                    get_at(size() - 2).setSuffixComment(comments[0]);
                    DEBUG_LOG("ElementArray: inner Element(penultimate) add SuffixComment: " << "\""
                              << get_at(size() - 2).getSuffixComment() << "\"");
                }
                comments.erase(comments.cbegin());
            } else {
                if(get_back().getSuffixComment().empty())
                {
                    get_back().setSuffixComment(comments[0]);
                    DEBUG_LOG("ElementArray: inner Element(back) add SuffixComment: " << "\""
                              << get_back().getSuffixComment() << "\"");
                }
                comments.pop_back();
            }
        }
    };

    for(size_t i = 0; i < content.size() && state != ParseStateJsonArray::eARRAY_ERROR_STATE; i++) {
        char ch_previous    = i == 0 ? 0 : content[i - 1];
        char ch_current     = content[i];
        char ch_next        = i < content.size() ? content[i + 1] : 0;

        counter.check(i, ch_current); //TODO: написать тест для проверки счётчика символов

        //поиск комментариев ===================================================
        //вернёт комментарий без обрамления
        try {
            CheckComments(ch_current, ch_next, i, getCommentDesign(), current_comment, stacker.empty());
        } catch(std::exception& e) {
            //не хватило символов для прочтения комментария
            CreateError(counter, e.what());
            break;
        }

        if(!getCommentDesign().with_comments)
            current_comment.clear();
        if(getCommentDesign().with_comments && getCommentDesign().temp_type == CommentType::eCommentEnd)
        {
            comments.push_back(FromComment(std::move(current_comment), getCommentDesign()));
            current_comment.clear();
            getCommentDesign().temp_type = CommentType::eNotComment;
            continue;
        }
        if(getCommentDesign().temp_type != CommentType::eNotComment)
            continue;
        //=================================================== поиск комментариев

        // обработка комментария ПОСЛЕ значения
        if(utils::CharInString(ch_current, __SEPARATORS__)) {
            if(state == ParseStateJsonArray::eARRAY_VALUE
                && key.empty()
                && !comments.empty()
                && (last_separator_symbol == ','
                    || (ch_current == '\n' && last_separator_symbol == ',' && comments[0].find('\n') == std::string::npos)))
            {
                //для отладки
                // std::cout << "last_separator_symbol: '"
                //           << (last_separator_symbol == '\n' ? "\\n" : std::string(&last_separator_symbol, 1))
                //           << "'" << std::endl;
                AppendElementSuffixComment();
            }
            /* таким образом запоминаются последние два разделителя:
             * - last_separator_symbol  - предыдущий
             * - ch_current             - текущий
             */
            last_separator_symbol = ch_current;
        }

        switch(state) {
        case ParseStateJsonArray::eARRAY_START: {
            //пропуск пробелов =====================================================
            if(CharInString(ch_current, __SPACES__)) break;
            //===================================================== пропуск пробелов

            if(ch_current == '[') {
                // работа с комментариями (до разбора json)
                AppendMainPreviewComment();

                UpdateState(state, ParseStateJsonArray::eARRAY_VALUE);
                break;
            }

            CreateError(counter, "not found start of JSON-ARRRAY structure");
            UpdateState(state, ParseStateJsonArray::eARRAY_ERROR_STATE);
            break;
        }
        case ParseStateJsonArray::eARRAY_VALUE: {
            //игнор пробелов и разделителей пока значение пустое
            if(stacker.empty()
                && value.empty()
                && CharInString(ch_current, __SPACES__))
            {
                break;
            }
            if(stacker.empty()
                && value.empty()
                && ch_current == ']') {
                // работа с комментарием после элемента
                if(!isEmpty() && get_back().getSuffixComment().empty())
                    AppendElementSuffixComment();

                UpdateState(state, ParseStateJsonArray::eARRAY_FINISH);
                break;
            }

            if(ch_previous != '\\')
            {
                if(!stacker.autocheck(ch_current)) {
                    CreateErrorUnexpected(counter, ch_current);
                    UpdateState(state, ParseStateJsonArray::eARRAY_ERROR_STATE);
                    break;
                }
            }

            if(!stacker.empty() || !CharInString(ch_current, __SEPARATORS__))
                value += ch_current;
            else {
                i--;
            }

            //значение прочитано полностью?
            if(stacker.empty()
                && (CharInString(ch_next, __SEPARATORS__ " ]")
                        || ch_next == 0
                        || CharInString(ch_current, __SEPARATORS__))
                && (value.empty() || !utils::OnlySpaces(value))
                )
            {
                DEBUG_LOG("ElementArray: current value done: \"" << value << "\"");

                Config element = CreateElementFromString(std::move(value), ConfigFormat::eJSON, getCommentDesign(), start_value_counter);
                if(element.error()) {
                    //если случилась ошибка при внутренней конвертации прочитанного значения,
                    // то эта ошибка становится основной ошибкой парсинга
                    CreateError(counter, "value parse error: " + element.getError());
                    UpdateState(state, ParseStateJsonArray::eARRAY_ERROR_STATE);
                    break;
                }

                push_back(std::move(element));
                if(get_back().getCommentDesign().opt_multiline_column_size > getCommentDesign().opt_multiline_column_size)
                    getCommentDesign().opt_multiline_column_size = get_back().getCommentDesign().opt_multiline_column_size;
                key.clear();
                value.clear();

                // проверка замыкающего комментария (вторичная)
                if(value_read_at_line == counter.getLastLineCounter())
                {
                    if(!comments.empty()
                        && size() > 1
                        && get_at(size() - 2).getSuffixComment().empty()
                        && (getCommentDesign().temp_type == CommentType::eCommentEnd || getCommentDesign().temp_type == CommentType::eNotComment))
                    {
                        get_at(size() - 2).setSuffixComment(comments[0]);
                        DEBUG_LOG("ElementJson: inner Element add SuffixComment: " << "\"" << get_at(size() - 2).getSuffixComment() << "\"");
                        comments.erase(comments.cbegin());
                    }
                }

                // проверка замыкающего комментария (вторичная)
                if(value_read_at_line == counter.getLastLineCounter())
                    AppendElementSuffixComment(true);

                // работа с комментариями перед элементом
                AppendElementPrefixComment();

                UpdateState(state, ParseStateJsonArray::eARRAY_SEPARATOR);
            }

            break;
        }
        case ParseStateJsonArray::eARRAY_SEPARATOR: {
            //пропуск пробелов =====================================================
            if(CharInString(ch_current, __SPACES_WITHOUT_SEPARATORS__)) break;
            //===================================================== пропуск пробелов

            //может встретиться разделитель или знак завершения массива
            //  комментарий ПОСЛЕ значения может начинаться только на той же строке, что и значение
            //  разделитель может быть как ДО, так и ПОСЛЕ комментария
            //  если комментарий расписан после переноса строки, но до знака }, то комментарий попадёт в суффикс основы

            // запоминаем номер строки, на котором закончили считывать значение
            value_read_at_line = counter.getLastLineCounter(); //применится перед } и перед считыванием значения
            value.clear();

            if(CharInString(ch_current, __SEPARATORS__)) {
                UpdateState(state, ParseStateJsonArray::eARRAY_VALUE);
                break;
            }
            if(CharInString(ch_current, "]")) {
                UpdateState(state, ParseStateJsonArray::eARRAY_FINISH);
                break;
            }

            CreateError(counter, "not found end of JSON-ARRAY structure or value separator");
            UpdateState(state, ParseStateJsonArray::eARRAY_ERROR_STATE);
            break;
        }
        case ParseStateJsonArray::eARRAY_FINISH: {
            CreateErrorUnexpected(counter, ch_current, "after end of JSON-ARRAY structure there can only be comments");
            break;
        }
        default: break;
        }

        // любая ошибка означает выход из парсера
        if(state == ParseStateJsonArray::eARRAY_ERROR_STATE)
            break;
    }

    //конечный комментарий ...
    if(state == ParseStateJsonArray::eARRAY_FINISH)
    {
        AppendMainSuffixComment();
    }

    if(state != ParseStateJsonArray::eARRAY_FINISH && state != ParseStateJsonArray::eARRAY_VALUE) {
        if(!error())
            setError(error_template + "not found end of JSON-ARRAY structure or value separator"); //если встретили конец файла
        DEBUG_LOG("ERROR: " << getError());
        //NOTE: (ElementArray) в случае ошибки парсинга корректно прочитанные значения сохраняются
        //clear();
    }
}

Config &Config::GetFirstArrayFromThis(Config &config) noexcept
{
    //если был Null - преобразовать в ElementArray
    //если был любой элемент, кроме массива - создать массив и вернуть его
    //если был массив - вернуть его же

    switch(config.getType()) {
    case ValueType::eNull: {
        config = Config(ValueType::eArray);
        return config;
    }
    case ValueType::eArray: {
        return config;
    }
    default: {
        Config temp = config;
        config.setValue(Config(ValueType::eArray));
        config.push_back(temp);
        return config;
    }
    }
}

Config &Config::GetFirstJsonFromThis(Config &config) noexcept
{
    //если был Null - преобразовать в ElementJson
    //если был примитив - создать массив и положить внутрь примитив, после добавить ElementJson и вернуть его
    //если был массив - найти первый попавшийся ElementJson
    //если элемент уже ElementJson - вернуть его же

    switch(config.getType()) {
    case ValueType::eNull: {
        config = Config(ValueType::eJson);
        return config;
    }
    case ValueType::eArray: {
        for(auto& c : config.getRange()) {
            if(c->isJson()) {
                return *c;
            }
        }
        config.push_back(Config(ValueType::eJson));
        return config.get_back();
    }
    case ValueType::eJson: {
        return config;
    }
    default: {
        Config temp = config;
        config.setValue(Config(ValueType::eArray));
        config.push_back(temp);
        config.push_back(Config(ValueType::eJson));
        return config.get_back();
    }
    }
}

std::string simpleapi::Config::ToString(const ParseStateXml state) noexcept
{
    switch (state) {
        case ParseStateXml::eXML_FORMAT:        return "[XML_FORMAT]";
        case ParseStateXml::eXML_PROLOGUE:      return "[XML_PROLOGUE]";
        case ParseStateXml::eXML_TAG_START:     return "[XML_TAG_START]";
        case ParseStateXml::eXML_ATTRIBUTES:    return "[XML_ATTRIBUTES]";
        case ParseStateXml::eXML_INNER_VALUE:   return "[XML_INNER_VALUE]";
        case ParseStateXml::eXML_TAG_FINISH:    return "[XML_TAG_FINISH]";
        case ParseStateXml::eXML_EPILOGUE:      return "[XML_EPILOGUE]";
        case ParseStateXml::eXML_ERROR:
        default:                                return "[XML_ERROR]";
        }
}

void Config::UpdateState(ParseStateXml &state, const ParseStateXml new_state) noexcept
{
    state = new_state;
    DEBUG_LOG("Parse XML, upd state: " << ToString(state) << std::endl);
}

//функция должна быть вызвана исключительно для обработки строки значения, комменты не учитывает
Config CreateElementFromString(std::string &&value_string, const ConfigFormat format,
                               const CommentDesign &design, ParserSymbolCounter& start_iterator) noexcept
{
    using namespace utils;
    using namespace tools;

    //удаление незначащих пробелов
    RemoveIllegalSpaces(value_string);

    std::string temp;
    auto Append = [&](const char c) {
        temp += std::tolower(c);
    };

    //проверка типа, по порядку
    {
        /*NULL*/ {
            if(value_string.empty())
                return Config();
            if(value_string.size() == 4) {
                for(auto ch : value_string)
                    Append(ch);
                if(temp == "null")  return Config();
                temp.clear();
            }
        }
        /*BOOL*/ {
            try {
                return Config(utils::CreateBoolFromString(value_string));
            } catch(...) {}
            temp.clear();
        }
        char first = value_string.front();
        char last = value_string.back();
        /*NUMBER*/ {
            try {
                static std::regex reg("^[+-]?[0-9]*[.]?[0-9]*[eE]?[+-]?[0-9]*[fF]?$");
                if(std::regex_match(value_string, reg))
                    return Config(std::stold(value_string));
            } catch (...) {}

            //FIXME: переместить в парсер number_utils.h
            // // обработка бесконестей (inf/infinity/∞, -inf/-infinity/-∞)
            // std::string temp = value_string.substr(0, value_string.size() < 11 ? value_string.size() : 11); //размер от максимального "-infinity"
            // if(!temp.empty()) {
            //     // преобразование в lowercase
            //     std::transform(temp.begin(), temp.end(), temp.begin(), [](char c) { return std::tolower(c); });
            //     // удаление кавычек в начале и конце строки
            //     if(temp.front() == temp.back() && (temp.front() == '"' || temp.front() == '\'')) {
            //         temp.erase(0, 1);
            //         temp.pop_back();
            //     }
            //     bool is_negative = temp.front() == '-';
            //     if(is_negative) {
            //         temp.erase(0, 1);
            //     }
            //     if(temp == "inf" || temp == "infinity" || temp == "∞") {
            //         long double inf_d = is_negative ? -std::numeric_limits<long double>::infinity()
            //                                         : std::numeric_limits<long double>::infinity();
            //         return Config(inf_d);
            //     }
            // }
        }
        /*ARRAY*/ {
            if(first == '[') {
                Config array;
                if(last == ']') {
                    array.setCommentDesign(design);
                    array.parseFullJsonArrayDoc(std::string(value_string));
                } else {
                    //есть начало массива, но нет конца
                    array.setError("not found end of Json-array value");
                }
                return array;
            }
        }
        /*JSON*/ {
            if(first == '{') {
                Config json;
                if(last == '}') {
                    json.setCommentDesign(design);
                    json.parseFullJsonDoc(std::string(value_string));
                } else {
                    //есть начало Json, но нет конца
                    json.setError("not found end of Json value");
                }
                return json;
            }
        }
    }

    // в теории, всё, что не распарсилось в другие значения, - должно считаться строкой
    //удаление кавычек (при наличии)
    RemoveQuotes(value_string);
    /*CHAR*/ if(value_string.size() == 1) {
        return Config(value_string);
    }

    /*STRING*/
    return Config(value_string);
}

Config ReadFile(const std::string &file_path, const ConfigFormat format,
                const CommentDesign &design) noexcept
{
    switch(format) {
    case ConfigFormat::eJSON:   return ReadFileJson(file_path, design);
    case ConfigFormat::eINI:    return ReadFileIni(file_path, design);
//    case ConfigFormat::eYAML:
//    case ConfigFormat::eXML:
    default: break;
    }

    Config out;
    out.setError("unexpected ConfigFormat");
    return out;
}

Config ReadFileJson(const std::string &file_path, const CommentDesign &design) noexcept
{
    std::string input_str;
    std::string error_str;

    if(GetAllStringsFromFile(file_path, input_str, &error_str)) {
        return ParseJson(input_str, design);
    }

    Config out;
    out.setError(error_str);
    return out;
}

Config ReadFileIni(const std::string &file_path, const CommentDesign &design) noexcept
{
    std::string input_str;
    std::string error_str;

    if(GetAllStringsFromFile(file_path, input_str, &error_str)) {
        return ParseIni(input_str, design);
    }

    Config out;
    out.setError(error_str);
    return out;
}

Config ReadFileXml(const std::string &file_path, const CommentDesign &design) noexcept
{
    //TODO: ReadFileXml()
    return {};
}

//NOTE: eONLY_VALUE выводит переменную в формате JSON без пробелов и комментариев
bool WriteFile(const Config& config, const std::string& file_path,
               const ConfigFormat format, const CommentDesign &design,
               const uint8_t custom_tabulation_level) noexcept
{
    return WriteStringToFile(file_path, config.toString(format, design, custom_tabulation_level));
}

bool WriteFileJson(const Config& config, const std::string& file_path,
                   const CommentDesign &design, const uint8_t custom_tabulation_level) noexcept
{
    return WriteStringToFile(file_path, config.toString(ConfigFormat::eJSON, design, custom_tabulation_level));
}

bool WriteFileIni(const Config& config, const std::string& file_path,
                  const CommentDesign &design, const uint8_t custom_tabulation_level) noexcept
{
    return WriteStringToFile(file_path, config.toString(ConfigFormat::eINI, design, custom_tabulation_level));
}

bool WriteFileXml(const Config& config, const std::string& file_path,
                  const CommentDesign &design, const uint8_t custom_tabulation_level) noexcept
{
    return WriteStringToFile(file_path, config.toString(ConfigFormat::eXML, design, custom_tabulation_level));
}

Config Parse(const std::string &content, const ConfigFormat format,
                              const CommentDesign &design) noexcept
{
    switch(format) {
    case ConfigFormat::eJSON:   return ParseJson(content, design);
    case ConfigFormat::eINI:    return ParseIni(content, design);
//    case ConfigFormat::eYAML:   return ParseYaml(content, with_comments);
//    case ConfigFormat::eXML:    return ParseXml(content, with_comments);
    default:                    break;
    }

    Config out;
    out.setError("unexpected ConfigFormat");
    return out;
}

Config ParseJson(const std::string &content, const CommentDesign &design) noexcept
{
    Config ret(ValueType::eJson);

    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется соб
        n_design = ret.getCommentDesign();
    }

    ret.parseJson(content, n_design);
    return ret;
}

Config ParseIni(const std::string &content, const CommentDesign &design) noexcept
{
    Config ret(ValueType::eJson);

    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется соб
        n_design = ret.getCommentDesign();
    }

    ret.parseIni(content, n_design);
    return ret;
}

Config ParseXml(const std::string &content, const CommentDesign &design) noexcept
{
    Config ret(ValueType::eXml);

    CommentDesign n_design = design;
    if(design == CommentDesign{}) { // если переменная не заполнена, используется соб
        n_design = ret.getCommentDesign();
    }

    ret.parseXml(content, n_design);
    return ret;
}

} // namespace simpleapi
