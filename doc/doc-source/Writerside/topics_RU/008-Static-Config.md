# 8. Static Config

## Об идее
В ходе разработки и тестирования возможностей библиотеки SimpleAPI была сформирована идея, что класс 
Config позволяет многое, но упускает иногда главное для программиста, работающего с файлами конфигураций. 
А именно - подсветку доступных переменных при обращении к объекту Config внутри IDE.

Проблема исходит из того, что объект Config формируется на ходу, то есть ему совершенно неважно, что вы 
дадите ему на вход.

В связи с этим появилась концепция, что программист-пользователь библиотеки может через макросы описать 
необходимые ему типы и названия полей структуры, а библиотека берёт на себя её заполнение и валидацию 
средствами класса Config и паттерн X-macro.

На данный момент эта технология поддерживает:
- одиночные примитивы (строки, числа, bool, char)
- вложенные структуры, описанные через эту же технологию
- перечисления (enum) - при этом в конечном файле конфига хранится строковое представление поля enum
- STL-контейнеры: 
    - `std::vector<T>`
    - `std::list<T>`
    - `std::forward_list<T>`
    - `std::deque<T>`
    - `std::set<T>`
    - `std::multiset<T>`
    - `std::unordered_set<T>`
    - `std::unordered_multiset<T>`
    - `std::queue<T>`
    - `std::priority_queue<T>`
    - `std::stack<T>`
    - `std::array<T, size>`
    - `std::bitset<size>`
- ассоциативные STL-контейнеры (K = строка, число или bool):
    - `std::map<K, T>`
    - `std::multimap<K, T>`
    - `std::unordered_map<K, T>`
    - `std::unordered_multimap<K, T>`
- `std::pair<K, T>`
- динамический ассоциативный массив `simpleapi::Config`

## Макросы описания
### X-Macro
Макросы этой группы целиком описываются пользователем, но суть в следующем. Каждый из описанных ниже макросов 
принимает на вход определённое количество параметров. Не все из них обязательные. Это как с вызовом функций,
у которых переменное количество параметров.

### SAPI_REGISTER_ENUM(EnumName, X_MACRO_FOR_CURRENT_ENUM_FIELDS, UnderlyingType)
**Параметры основного макроса:**
- `EnumName`
    - итоговое имя класса перечисления (enum class)
- `X_MACRO_FOR_CURRENT_ENUM_FIELDS`
    - макрос X-Macro, объявленный пользователем ранее, описывающий список значений
- `UnderlyingType`
    - (опциональный) базовый класс, от которого будет наследован enum class

**Параметры X-Macro:**
- `element`
    - имя поля enum class
- `value`
    - (опциональный) числовой параметр указанного значения

**Вспомогательные функции, которые создаёт основной макрос регистрации перечисления:**
- `std::string ToString(value)`
    - функция для перевода конкретного значения внутри enum class к строковому виду
- `void FromString(input_str, out_value)`
    - функция для преобразования строки в значение enum class; если значение не будет распознано - применится поле ::_UNDEFINED_STATE_
- `std::string EnumPossibleVariants(value)`
    - функция, которая выводит в строку все возможные валидные поля данного enum class

### SAPI_REGISTER_CONFIG(StructName, X_MACRO_FOR_CURRENT_STRUCT_FIELDS)
**Параметры основного макроса:**
- `StructName`
    - итоговое имя структуры
- `X_MACRO_FOR_CURRENT_STRUCT_FIELDS`
    - макрос X-Macro, объявленный пользователем ранее, описывающий список параметров

**Параметры X-Macro:**
- `type`
    - тип переменной, которая будет описана внутри структуры
- `name`
    - имя переменной; будет указано и внутри структуры, и в итоговом файле Config
- `default_value`
    - значение по умолчанию, будет применено ещё до вызова .loadConfig()
- `lambda`
    - (опциональный) лямбда-функция, которая принимает на вход аргумент `const type& val`, описывающий тип переменной;
    значение будет применено к прочитанному библиотекой значением из файла конфига
- `prefix_comment`
    - (опциональный) строка комментария, которая будет выведена в файле конфига на строке перед значением
- `suffix_comment`
    - (опциональный) строка комментария, которая будет выведена в файле конфига на строке значения сразу после него

**ВАЖНО: если поле внутри себя должно иметь запятые, то нужно использовать либо `using XXX = ...;` , либо скобки `(...)`**

**Вспомогательные функции, которые создаёт основной макрос регистрации структуры:**
- `bool loadConfig(const simpleapi::Config& config)`
    - заполнит структуру значениями из config; вернёт true если все поля были прочитаны корректно и провалидировано 
    лямбдой (при наличии)
    - в случае ошибки загрузки значения заполнит глобальную строку `simpleapi::static_config_error_str`
- `simpleapi::Config saveConfig() const`
    - сохранит значения полей структуры в объект Config

## Примеры использования
### Объявление enum class
```c++
// объявление X-Mac
#define MY_ENUM_FIELDS(X) \
    X(eVal_1)             \
    X(eVal_2)

// создаст enum class { eVal_1, eVal_2, _UNDEFINED_STATE_ };
SAPI_REGISTER_ENUM(MyEnum, MY_ENUM_FIELDS)

// создаст enum class : uint8_t { eVal_1, eVal_2, _UNDEFINED_STATE_ };
SAPI_REGISTER_ENUM(MyEnum2, MY_ENUM_FIELDS, uint8_t)
```

### Объявление struct
```c++
// объявление X-Macro
// поле int_value описывает только обязательные параметры
// поле str_value имеет функцию валидации, которая проверяет минимальную длину прочитанной строки
// поле int_vec не имеет валидации, но имеет комментарий перед значением
#define MY_STRUCT_FIELDS(X)                                              \
    X(int,              int_value, 0)                                    \
    X(std::string,      str_value, "bla-bla-bla",                        \
        [](const std::string& s) -> bool { return s.size() > 10; }       \
    X(std::vector<int>, int_vec,   {}, nullptr, "is vector of integers") \

// создаст struct MyStaticConfig с полями, описанными выше
SAPI_REGISTER_CONFIG(MyStaticConfig, MY_STRUCT_FIELDS)

// описание структуры, которая внутри содержит внутреннюю структуру, объявленную ранее
#define MY_STRUCT2_FIELDS(X)                         \
    X(char,           ch,           'a')             \
    X(MyStaticConfig, inner_struct, MyStaticConfig{})

SAPI_REGISTER_CONFIG(MyStaticConfig2, MY_STRUCT2_FIELDS)
```

### Применение в коде
Для примера берём структуру из предыдущего кода:
```c++
MyStaticConfig2 static_config;

// loadConfig() вернёт исключение, если пользователь применил некорректный тип переменной при заполнении
try {
	if(!static_config.loadConfig())
	{
		std::cerr << "Error: incorrect config!" << std::endl;
	}
} catch (...) {}
```


### Пример использования внешней переменной в лямбде
Стандарт C++11 не позволяет передавать владение глобальным объектом внутрь лямбды.
Зато позволяет её явное использование.

```c++
#include <...> // включение необходимых библиотек на усмотрение разработчика

std::string my_global_string;

// объявляем Static Config структуру
#define FIELDS(X)                                                       \
    X(std::vector<int>, vec, std::vector<int>,                          \
        [](const std::vector<int>& v) -> bool {                         \
            bool b = v.size() >= 10;                                    \
            if(b) {                                                     \
                return true; /*успех валидации*/                        \
            }                                                           \
            /* заполняем строку ошибки */                               \
            my_global_string = "В поле \"vec\" слишком мало элементов!" \
            return false; /* выходим с ошибкой валидации */             \
        } )

SAPI_REGISTER_CONFIG(MyStruct, FIELDS)

int main() {
    using simpleapi;
    // считываем конфигурацию
    Config cfg = ReadIniFile("path/to/my/conf.ini");
    
    // заполняем статическую структуру на основе динамической
    MyStruct ms;
    try {
        if(!ms.loadConfig(cfg))
        {
            // при загрузке произошла ошибка валидации, выводим наше сообщение
            std::cout << "MyStruct loading error.";
            if(!my_global_string.empty())
                std::cout << " " << my_global_string;
            std::cout << std::endl;
            return 1;
        }
    } catch(...) {}
    
    return 0;
}
```
