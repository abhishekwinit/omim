include_directories(${CMAKE_HOME_DIRECTORY}/3party/jansson/src ${OMIM_DIR}/platform)

set(
    SRC
    chunks_download_strategy.cpp
    chunks_download_strategy.hpp
    constants.hpp
    country_defines.cpp
    country_defines.hpp
    country_file.cpp
    country_file.hpp
    file_logging.cpp
    file_logging.hpp
    get_text_by_id.cpp
    get_text_by_id.hpp
    http_request.cpp
    http_request.hpp
    http_thread_callback.hpp
    local_country_file.cpp
    local_country_file.hpp
    local_country_file_utils.cpp
    local_country_file_utils.hpp
    location.hpp
    measurement_utils.cpp
    measurement_utils.hpp
    mwm_traits.cpp
    mwm_traits.hpp
    mwm_version.cpp
    mwm_version.hpp
    platform.cpp
    platform.hpp
    preferred_languages.cpp
    preferred_languages.hpp
    servers_list.cpp
    servers_list.hpp
    settings.cpp
    settings.hpp
)


if(${PL_IPHONE})
    set(
        SRC
        ${SRC}
        http_thread_apple.h
        http_thread_apple.mm
        platform_ios.mm
        platform_unix_impl.cpp
        platform_unix_impl.hpp
    )
elseif(${PL_ANDROID})
    set(
        SRC
        ${SRC}
        platform_android.cpp
        platform_unix_impl.cpp
        platform_unix_impl.hpp
    )
else() # neither iPhone nor Android
    set(
        SRC
        ${SRC}
        location_service.cpp
        location_service.hpp
        platform_qt.cpp
        wifi_info.hpp
        wifi_location_service.cpp
   )

    if (${PL_WIN})
        set(
            SRC
            ${SRC}
            http_thread_qt.cpp
            http_thread_qt.hpp
            platform_win.cpp
            wifi_info_windows.cpp
        )
    elseif(${PL_MAC})
        set(
            SRC
            ${SRC}
            apple_location_service.mm
            http_thread_apple.h
            http_thread_apple.mm
            platform_mac.mm
            platform_unix_impl.cpp
            platform_unix_impl.hpp
        )
    elseif(${PL_LINUX})
        set(
            SRC
            ${SRC}
            http_thread_qt.cpp
            http_thread_qt.hpp
            platform_linux.cpp
            platform_unix_impl.cpp
            platform_unix_impl.hpp
        )
    endif()
endif()


add_subdirectory(platform_tests_support)

add_library(platform ${SRC})

omim_add_test_subdirectory(platform_tests)
