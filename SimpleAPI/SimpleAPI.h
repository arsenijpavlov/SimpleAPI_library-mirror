#pragma once

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "VersionInfo.h"                // IWYU pragma: keep
#include "utils/utils.h"                // IWYU pragma: keep
#include "utils/string_utils.h"         // IWYU pragma: keep
#include "utils/Timer.h"                // IWYU pragma: keep
#include "utils/Logger.h"               // IWYU pragma: keep

#include "config/Config.h"              // IWYU pragma: keep
#include "static/static_config.h"       // IWYU pragma: keep

#include "network/Socket.h"             // IWYU pragma: keep
#include "network/TcpSocket.h"          // IWYU pragma: keep
#include "network/UdpSocket.h"          // IWYU pragma: keep
#include "network/SocketThread.h"       // IWYU pragma: keep

#include "utils/file_utils.h"           // IWYU pragma: keep

#include "utils/Variant.h"              // IWYU pragma: keep


namespace simpleapi {

// функции для получения информации о библиотеке SimpleAPI
std::string GetVersion();
std::string GetVersionName();
std::string GetVersionCommit();
std::string GetFullVersionInfo();

//TODO: нужно обновление скрипта check_version_from_git.sh для параллельного выполнения в пределах Makefile (QMake)
//TODO: описать в документации версионирование проекта

} // namespace simpleapi
