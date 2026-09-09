#include "ElementArray.h"

#include <algorithm>
#include <stdexcept>
#include "../utils/utils.h"
#include "../utils/string_utils.h"
#include "../utils/Logger.h"

//предобъявление
#include "Config.h"
#include "ConfigDefines.h"
#include "ElementArray.h"


namespace simpleapi {
namespace tools {


void ElementArray::set_comment(const size_t& index, const Comment &content) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->setComment(content);
}

void ElementArray::set_comment(const size_t& index, const std::string &content_before,
                               const std::string &content_after)
{
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->setComment(content_before, content_after);
}

void ElementArray::set_prefix_comment(const size_t& index, const std::string &content) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->setPrefixComment(content);
}

void ElementArray::set_suffix_comment(const size_t& index, const std::string &content) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->setSuffixComment(content);
}

Comment &ElementArray::get_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    return m_values.at(index)->getComment();
}

Comment ElementArray::get_comment(const size_t& index) const {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    return m_values.at(index)->getComment();
}

std::string ElementArray::get_prefix_comment(const size_t& index) const {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    return m_values.at(index)->getPrefixComment();
}

std::string ElementArray::get_suffix_comment(const size_t& index) const {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    return m_values.at(index)->getSuffixComment();
}

//std::string &ElementArray::get_prefix_comment(const size_t& index) {
//    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
//    return m_values.at(index)->getPrefixComment();
//}

//std::string &ElementArray::get_suffix_comment(const size_t& index) {
//    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
//    return m_values.at(index)->getSuffixComment();
//}

void ElementArray::clear_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->clearComment();
}

void ElementArray::clear_prefix_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->clearPrefixComment();
}

void ElementArray::clear_suffix_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->clearSuffixComment();
}

void ElementArray::delete_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->deleteComment();
}

void ElementArray::delete_prefix_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->deletePrefixComment();
}

void ElementArray::delete_suffix_comment(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    m_values.at(index)->deleteSuffixComment();
}

void ElementArray::append_if_not_contains(const Config &config) noexcept {
    if(!contains(config))
        m_values.push_back(std::make_shared<Config>(config));
}

void ElementArray::append_if_not_contains(Config &&config) noexcept {
    if(!contains(config))
        m_values.push_back(std::make_shared<Config>(std::move(config)));
}

Config &ElementArray::get_front() {
    __CHECK_ARRAY_EMPTY_EXCEPTION__((*this))
    return *m_values.front();
}

Config ElementArray::get_front() const {
    __CHECK_ARRAY_EMPTY_EXCEPTION__((*this))
    return *m_values.front();
}

Config &ElementArray::get_at(const size_t& index) {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    return *m_values[index];
}

Config ElementArray::get_at(const size_t& index) const {
    __CHECK_CONTAINER_INDEX_CORRECT__((*this), index)
    return *m_values[index];
}

Config &ElementArray::get_back() {
    __CHECK_ARRAY_EMPTY_EXCEPTION__((*this))
    return *m_values.back();
}

Config ElementArray::get_back() const {
    __CHECK_ARRAY_EMPTY_EXCEPTION__((*this))
    return *m_values.back();
}

void ElementArray::clear() noexcept {
    clearComment();
    m_values.clear();
}

void ElementArray::insert_front(const VElement &elements) noexcept {
    for(size_t i = 0; i < elements.size(); i++)
        m_values.insert(cbegin() + i, std::make_shared<Config>(elements[i]));
}

void ElementArray::insert_front(VElement &&elements) noexcept {
    for(size_t i = 0; i < elements.size(); i++)
        m_values.insert(cbegin() + i, std::make_shared<Config>(std::move(elements[i])));
}

//если индекс больше существующего - добавить в конец массива
void ElementArray::insert_at(const size_t& index, const VElement &elements) noexcept {
    VElement temp_elements = elements;
    insert_at(index, std::move(temp_elements));
}

//если индекс больше существующего - добавить в конец массива
void ElementArray::insert_at(const size_t& index, VElement &&elements) noexcept {
    size_t counter = 0;
    for(auto &element : elements) {
        if(index+counter < size())
            m_values.insert(m_values.begin() + index + counter, std::make_shared<Config>(std::move(element)));
        else
            m_values.push_back(std::make_shared<Config>(std::move(element)));
        counter++;
    }
}

void ElementArray::insert_back(const VElement &elements) noexcept {
    std::transform(elements.cbegin(), elements.cend(), std::back_inserter(m_values),
                   [](const Config& element){ return std::make_shared<Config>(element); });
}

void ElementArray::insert_back(VElement &&elements) noexcept {
    std::transform(std::make_move_iterator(elements.begin()),
                   std::make_move_iterator(elements.end()),
                   std::back_inserter(m_values),
                   [](const Config& element) {
                       return std::make_shared<Config>(std::move(element));
                   });
}

shared_VElement::iterator ElementArray::insert_at(const shared_VElement::iterator iterator, const Config &value) {
    Config temp = value;
    return insert_at(iterator, std::move(temp));
}

shared_VElement::iterator ElementArray::insert_at(const shared_VElement::iterator iterator, Config &&value) {
    if(iterator >= m_values.begin() && iterator < m_values.end())
        return m_values.insert(iterator, std::make_shared<Config>(std::move(value)));
    else
        return m_values.insert(m_values.end(), std::make_shared<Config>(std::move(value)));
}

void ElementArray::append(const ElementArray &other) noexcept {
    ElementArray array_copy(other);
    append(std::move(array_copy));
}

void ElementArray::append(ElementArray &&other) noexcept {
    for(auto& o : other.m_values)
        insert_back(std::move(*o.get()));
}

void ElementArray::append_null(size_t size) noexcept {
    for(size_t i = size; i > 0; --i)
        m_values.push_back(std::make_shared<Config>());
}

void ElementArray::pop_front() noexcept {
    if(!m_values.empty())
        m_values.erase(cbegin());
}

void ElementArray::pop_at(const size_t& index) noexcept {
    if(m_values.size() > index)
        m_values.erase(cbegin() + index);
}

void ElementArray::pop_back() noexcept {
    if(!m_values.empty())
        m_values.pop_back();
}

Config ElementArray::get_and_pop_front() {
    Config cfg = get_front();
    pop_front();
    return cfg;
}

Config ElementArray::get_and_pop_at(const size_t& index) {
    Config cfg = get_at(index);
    pop_at(index);
    return cfg;
}

Config ElementArray::get_and_pop_back() {
    Config cfg = get_back();
    pop_back();
    return cfg;
}

shared_VElement::iterator ElementArray::pop_at(const shared_VElement::iterator iterator)
{
    if(iterator >= m_values.begin() && iterator < m_values.end())
        return m_values.erase(iterator);
    else
        return m_values.end();
}

bool ElementArray::isEqual(const IElement &other, const bool compare_comments,
                           const bool map_sort_important) const noexcept
{
    bool b1 = true;
    if(compare_comments)
        b1 = isCommentsEqual(other);

    bool b2 = size() == other.size();
    if(b2) {
        const ElementArray& other_array = reinterpret_cast<const ElementArray&>(other);
        for(size_t i = 0; i < size(); i++) {
            if(!m_values[i]->isEqual(*other_array.m_values[i], compare_comments, map_sort_important)) {
                b2 = false;
                break;
            }
        }
    }

    return b1 && b2;
}

//комментарии при поиске не учитываются
bool ElementArray::contains(const Config &config) const noexcept {
    return std::any_of(cbegin(), cend(), [&config](const std::shared_ptr<Config> &value){ return value->isEqual(config); });
}

Config ElementArray::operator[](const size_t &index) const    {
    return get_at(index);
}

bool ElementArray::operator==(const Config &config) const
{
    if(!config.isArray())
        return false;

    if(size() != config.size())
        return false;

    for(size_t i = 0; i < size(); i++) {
        if((*this)[i] != config[i])
            return false;
    }

    return true;
}

std::string ElementArray::toString(const ConfigFormat format, const CommentDesign &design,
                                   const int8_t custom_tabulation_level) const noexcept
{
    switch(format){
    //предполагается, что eONLY_VALUE - это JSON без пробелов и переносов строки
    case ConfigFormat::eONLY_VALUE: return toJsonString(design, -1);
    case ConfigFormat::eJSON:       return toJsonString(design, custom_tabulation_level);
    case ConfigFormat::eINI:        return toIniString(design, custom_tabulation_level);
    case ConfigFormat::eYAML:       return toYamlString(design, custom_tabulation_level);
    case ConfigFormat::eXML:        return toXmlString(design, custom_tabulation_level);
    default:                        return "[UNKNOWN_CONFIG_FORMAT]";
    }
}

std::string ElementArray::toJsonString(const CommentDesign &design, const int8_t custom_tabulation_level) const noexcept
{
    std::string ret;
    const std::string tablulation_str   = utils::RepeatSymToStr('\t', custom_tabulation_level);
    const std::string tablulation_str_1 = utils::RepeatSymToStr('\t', custom_tabulation_level + 1);

    //(для отладки)
    //ret += "custom_tabulation_level:" + std::to_string(custom_tabulation_level) + "\n";

    bool with_spaces = custom_tabulation_level != -1;
    CommentDesign inner_design   = design;
    inner_design.is_in_container = true;

    //вывод комментария с рамкой
    if(with_spaces
        && design.with_comments
        && !design.is_in_container
        && !getPrefixComment().empty())
    {
        ret += ToComment(getPrefixComment(), inner_design, custom_tabulation_level);
        ret += "\n";
    }

    if(with_spaces)
        ret += tablulation_str;
    ret += "[";
    if(with_spaces)
        ret += "\n";

    for(size_t i = 0; i < size(); i++)
    {
        //вывод комментария с рамкой
        if(with_spaces
            && design.with_comments
            && !m_values[i]->getPrefixComment().empty())
        {
            //вернули в исходное состояние после записи КОММЕНТАРИЯ ПОСЛЕ ЗНАЧЕНИЯ
            inner_design.opt_multiline_column_size = design.opt_multiline_column_size;

            //NOTE: если переменная содержит комментарий ДО значения - делаем пустую строку для лучшей читабельности
            if(i != 0) //из-за табуляции для первого элемента итоговый стиль смотрится странно
                ret += "\n";

            ret += ToComment(m_values[i]->getPrefixComment(), inner_design, custom_tabulation_level + 1);
            ret += "\n";
        }

        //вывод значения
        if(with_spaces)
            ret += tablulation_str_1;

        std::string temp = m_values[i]->toString(ConfigFormat::eJSON, inner_design,
                                                 (with_spaces ? custom_tabulation_level + 1 : -1));
        if(m_values[i]->isContainer()) {
            ret += utils::RemoveStartTabulations(temp);
        } else {
            if(!m_values[i]->isString() && !m_values[i]->isChar()) {
                //в числах могут быть бесконечности
                if(m_values[i]->isNumber()) {
                    if(std::any_of(temp.begin(), temp.end(),
                                    [](char c) {
                                        switch(c) {
                                        case '0':
                                        case '1':
                                        case '2':
                                        case '3':
                                        case '4':
                                        case '5':
                                        case '6':
                                        case '7':
                                        case '8':
                                        case '9':
                                        case '-':
                                        case '.':   return false;
                                        default:    return true;
                                        }
                                    }
                                    )
                        )
                    {
                        ret += "\"" + temp + "\"";
                    } else {
                        ret += temp;
                    }
                } else {
                    ret += temp;
                }
            } else {
                utils::SplittedLines sl = utils::SplitWithoutColumned(temp);
                // все строки кроме первой выровнять по первой строке
                for(size_t j = 1; j < sl.lines.size(); j++) {
                    sl.lines[j] = utils::RepeatSymToStr('\t', custom_tabulation_level + 1) + sl.lines[j];
                }

                ret += "\"";
                ret += utils::VStringToString(sl.lines);
                ret += "\"";
            }
        }

        if(i < size() - 1)
            ret += ",";

        //вывод комментария без рамки
        if(with_spaces
            && inner_design.with_comments
            && !m_values[i]->getSuffixComment().empty())
        {
            //NOTE: (ширина колонки многосторчного комментария после значения не влияет на вывод)
            inner_design.opt_multiline_column_size = 0;

            std::string temp = ToComment(m_values[i]->getSuffixComment(), inner_design, -1);
            const size_t pos = ret.rfind('\n');
            std::string temp__ = (ret.size() > pos +1 ? (ret.substr(pos + 1, ret.size())) : "") + " ";
            utils::SetStringAsOnlySpaces(temp__);
            utils::AddStringForFromLine(temp, 2, temp__);
            ret += " " + temp;
        }

        if(with_spaces)
            ret += "\n";
    }

    if(with_spaces)
        ret += tablulation_str;
    ret += "]";

    //вывод комментария без рамки
    if(with_spaces
        && design.with_comments
        && !design.is_in_container
        && !getSuffixComment().empty())
    {
        //NOTE: (ширина колонки многосторчного комментария после значения не влияет на вывод)
        inner_design.opt_multiline_column_size = 0;

        std::string temp = ToComment(getSuffixComment(), inner_design);
        const size_t pos = ret.rfind('\n');
        std::string temp__ = (ret.size() > pos +1 ? (ret.substr(pos + 1, ret.size())) : "") + " ";
        utils::SetStringAsOnlySpaces(temp__);
        utils::AddStringForFromLine(temp, 2, temp__);
        ret += " " + temp;
    }

    return ret;
}


// NOTE: метод не рекурсивный для контейнеров!
// NOTE: выравнивание на первом уровне ключей (второй уровень в рамках группы)
// NOTE: потому что нельзя мешать пользователю прострелить себе колено (не предполагается к использованию)
std::string ElementArray::toIniString(const CommentDesign &design, const int8_t custom_tabulation_level) const noexcept
{
    std::string ret;
    std::string temp;
    CommentDesign inner_design = design;

    auto GetPrefixComment = [&design](const Config cfg) -> std::string {
        return (!design.with_comments || cfg.getPrefixComment().empty()) ? ""
                                                                         : (ToComment(cfg.getPrefixComment(), design) + "\n");
    };
    auto GetSuffixComment = [&design, &ret, &inner_design](const Config cfg) -> std::string {
        if(!design.with_comments || cfg.getSuffixComment().empty())
            return "";

        // ширина колонки многострочного комментария после значения не влияет на вывод
        inner_design.opt_multiline_column_size = 0;

        utils::SplittedLines sl = utils::SplitWithoutColumned(ToComment(cfg.getSuffixComment(), inner_design, -1));
        //начиная со второй, все строки дополнить пробелами в начале по длине
        // последней строки +1(отделение комментария от значения)
        size_t pos = ret.rfind('\n');
        std::string last_line = (pos == std::string::npos) ? ret
                                                           : (ret.substr(pos + 1, ret.size()));
        size_t end_line_size = utils::GetStringCharCount(last_line) + 1;
        for(size_t i = 0; i < sl.lines.size(); i++) {
            if(i != 0) {
                sl.lines[i] = utils::RepeatSymToStr(' ', end_line_size) + sl.lines[i];
            }
        }

        return (cfg.getSuffixComment().empty()) ? "" : " " + utils::VStringToString(sl.lines);
    };
    auto IsArrayWithPrimitives = [](const Config cfg) -> bool {
        if(!cfg.isIndexContainer())
            return false;
        for(const auto& cfg_inner : cfg.getRange()) {
            if(cfg_inner->isContainer()
                || !cfg_inner->getPrefixComment().empty()
                || !cfg_inner->getSuffixComment().empty()
                || ((cfg_inner->isString() || cfg_inner->isChar())
                    && (cfg_inner->getString().find('\n') != std::string::npos
                        || cfg_inner->getString().size() > 50)))
            {
                return false;
            }
        }
        return true;
    };
    auto AppendMultinlineString = [&ret](const std::string& str) -> void {
        ret += "\"";
        std::string temp_str = str;
        if(temp_str.find('\n') != std::string::npos) {
            utils::SplittedLines sl = utils::SplitWithoutColumned(temp_str);
            for(size_t i = 0; i < sl.lines.size(); i++) {
                ret += (i == 0 ? "" : "    ");
                if(i + 1 < sl.lines.size())
                    ret += sl.lines[i] + " \\\n";
                else
                    ret += sl.lines[i];
            }
        } else {
            ret += str;
        }
        ret += "\"";
    };
    auto AppendArrayPrimitives = [&](const std::string& key, Config& cfg) -> void {
        if(!key.empty())
            ret += key + " = ";

        ret += "[";
        for(size_t i = 0; i < cfg.size(); i++) {
            if(cfg[i].isString() || cfg[i].isChar())
                ret += "\"";
            ret += cfg[i].toString();
            if(cfg[i].isString() || cfg[i].isChar())
                ret += "\"";

            if(i + 1 < cfg.size())
                ret += ", ";
        }
        ret += "]";
    };
    auto AppendCollection = [&](const VString& prefixes, Config& cfg) -> void {
        std::vector<std::unique_ptr<KeysBase>> kbss = CollectKeys(cfg, prefixes);
        size_t max_key_length = 0; // максимум текущей группы

        /* для реализации выравнивания знаков '=' нужно в рамках общей длины XXX/K рассчитать максимум длины K
         *  XXX - весь дополнительный контекст пути
         */
        {
            size_t index_start    = 0; // начальный индекс текущей группы (финишный по текущей позиции)
            uint16_t current_group_name_size = 0;
            for(size_t i = 0; i < kbss.size(); i++) {
                KeysValues* k_ptr = dynamic_cast<KeysValues*>(kbss[i].get());
                if (k_ptr) {
                    // проверить текущую длину XXX
                    std::string& current_full_key = k_ptr->m_key;
                    size_t XXX_size = current_full_key.rfind('/');
                    XXX_size        = XXX_size != std::string::npos ? XXX_size : 0;

                    auto Format = [&kbss, &max_key_length, &XXX_size]
                        (size_t i_start, size_t i_finish) -> void
                    {
                        for(size_t i = i_start; i <= i_finish; i++) {
                            KeysValues* k_ptr = dynamic_cast<KeysValues*>(kbss[i].get());
                            if(k_ptr)
                            {
                                std::string& key = k_ptr->m_key;
                                key              = logs::columned(key, XXX_size + max_key_length);
                            }
                        }
                    };

                    // если изменилась, значит мы перешли к следующей группе объектов
                    if(XXX_size != current_group_name_size)
                    {
                        // нужно пройтись по диапазону заново и выровнять по max_length
                        // если группа состоит из одного элемента - игнорить
                        Format(index_start, i);

                        max_key_length          = 0; // обнуляем для следующей группы
                        index_start             = i;
                        current_group_name_size = XXX_size; // обновляем размер текущей группы
                    }

                    // вычисляем максимум
                    if(max_key_length < current_full_key.size() - XXX_size)
                    {
                        if(m_writer_stile.max_key_length == -1
                            || (current_full_key.size() - XXX_size) <= m_writer_stile.max_key_length)
                        {
                            max_key_length = current_full_key.size() - XXX_size;
                        }
                    }

                    // встретили последний элемент - проходимся по последнему необработанному диапазону
                    if(i + 1 >= kbss.size()) {
                        Format(index_start, i);
                    }
                }
            }
        }

        for(auto& kbs : kbss) {
            KeysComments* ptr_comment = dynamic_cast<KeysComments*>(kbs.get());
            KeysValues* ptr_cfg       = dynamic_cast<KeysValues*>(kbs.get());
            if(design.with_comments && ptr_comment) {
                //групповой комментарий для INI так и или иначе будет напечатан с новой строки, т.к. потеряется привязанность к группе
                ret += ToComment(ptr_comment->m_comment_str, design) + "\n";
            }
            if(ptr_cfg) {
                if(!ptr_cfg->m_ptr_remote_cfg->isContainer())
                    ret += GetPrefixComment(*ptr_cfg->m_ptr_remote_cfg);

                if(!ptr_cfg->m_key.empty())
                    ret += logs::columned(ptr_cfg->m_key, max_key_length /*+ utils::GetStringCharCount(prefix)*/) + " = ";
                if(ptr_cfg->m_ptr_remote_cfg->isString() || ptr_cfg->m_ptr_remote_cfg->isChar()) {
                    AppendMultinlineString(ptr_cfg->m_ptr_remote_cfg->toString());
                } else {
                    ret += ptr_cfg->m_ptr_remote_cfg->toString();
                }

                if(!ptr_cfg->m_ptr_remote_cfg->isContainer()) {
                    temp = GetSuffixComment(*ptr_cfg->m_ptr_remote_cfg);
                    ret += std::move(temp);
                    ret += "\n";
                }
            }
        }
    };

    if(design.with_comments && !getPrefixComment().empty()) {
        ret += ToComment(getPrefixComment(), design) + "\n\n\n";
    }

    for(const auto& cfg : m_values) {
        bool it_was_array = cfg->isArray();
        switch(cfg->getType()) {
        case ValueType::eArray: {
            //все массивы первого уровня должны быть преобразованы в безымянный Json
            Config temp = *cfg;
            *cfg = Config(ValueType::eJson);
            cfg->push_back("", temp);
        }
        case ValueType::eJson: {
            ret += GetPrefixComment(*cfg);

            ret += ""; //контейнер в главном списке не может содержать ключа

            // в рамках группы рассчитать для одиночных элементов (не структур) максимальную длину имени
            // при записи дополнять пробелами до максимальной длины
            uint16_t max_length_inner = 0;
            for(const auto& cfg_inner : cfg->getNamedRange()) {
                if((!cfg_inner.second->isContainer() || IsArrayWithPrimitives(*cfg_inner.second))
                    && cfg_inner.first.size() > max_length_inner) {
                    max_length_inner = cfg_inner.first.size();
                }
            }

            // проверка ограничений
            if(m_writer_stile.max_key_length != -1
                && m_writer_stile.max_key_length < max_length_inner)
            {
                max_length_inner = m_writer_stile.max_key_length;
            }

            for(const auto& cfg_inner : cfg->getNamedRange()) {
                if(cfg_inner.second->isContainer()) {

                    if(IsArrayWithPrimitives(*cfg_inner.second))
                    {
                        ret += GetPrefixComment(*cfg_inner.second);

                        //если внутри только примитивы без комментариев - вывести их в одну строку (строки длиной <=50)
                        AppendArrayPrimitives(logs::columned(cfg_inner.first, max_length_inner), *cfg_inner.second);

                        temp = GetSuffixComment(*cfg_inner.second);
                        ret += std::move(temp);
                        ret += "\n";
                    } else {
                        //комментарии элемента cfg_inner будут обработаны внутри рекурсивной функции

                        ret += " ";
                        //нужно собрать все элементы массива и упаковать в общее имя с переходом между уровнями
                        AppendCollection({"", cfg_inner.first}, *cfg_inner.second);
                    }

                } else if(cfg_inner.second->isString() || cfg_inner.second->isChar()) {
                    ret += GetPrefixComment(*cfg_inner.second);

                    if(!cfg_inner.first.empty())
                        ret += logs::columned(cfg_inner.first, max_length_inner) + " = ";
                    AppendMultinlineString(cfg_inner.second->toString());

                    temp = GetSuffixComment(*cfg_inner.second);
                    ret += std::move(temp);
                    ret += "\n";
                } else {
                    ret += GetPrefixComment(*cfg_inner.second);

                    if(!cfg_inner.first.empty())
                        ret += logs::columned(cfg_inner.first, max_length_inner) + " = ";
                    ret += cfg_inner.second->toString();

                    temp = GetSuffixComment(*cfg_inner.second);
                    ret += std::move(temp);
                    ret += "\n";
                }
            } // for()

            if(!it_was_array)
            {
                temp = GetSuffixComment(*cfg);
                ret += std::move(temp);
                ret += "\n";
            }

            break;
        }
        default: {
            ret += GetPrefixComment(*cfg);

            ret += cfg->toString() + "\n";

            temp = GetSuffixComment(*cfg);
            ret += std::move(temp);
            ret += "\n";
            break;
        }
        }
    } // loop for()

    if(design.with_comments && !getSuffixComment().empty()) {
        ret += "\n\n" + ToComment(getSuffixComment(), design);
    }

    return ret;
}

std::string ElementArray::toYamlString(const CommentDesign &design, const int8_t custom_tabulation_level) const noexcept
{
    //TODO (потом): std::string ElementArray::toYamlString()
    return "[TODO]";
}

std::string ElementArray::toXmlString(const CommentDesign &design, const int8_t custom_tabulation_level) const noexcept
{
    //TODO (потом): std::string ElementArray::toXmlString()
    return "[TODO]";
}

bool IsElementArray(const IElement &e) noexcept
{
    return e.getType() == ValueType::eArray;
}

bool IsElementArray(const Config &cfg) noexcept
{
    return cfg.isArray();
}

} // namespace tools
} // namespace simpleapi
