set(M3U8DOWNLOADER TestM3u8Downloader)

# 从文件夹从获取文件的决定路径
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# 添加动态库
add_executable(${M3U8DOWNLOADER} ${SOURCE_FILES})

target_compile_options(${M3U8DOWNLOADER} PRIVATE "/MT$<$<CONFIG:Debug>:d>")

# 添加包含目录
target_include_directories(${M3U8DOWNLOADER} PRIVATE ${CMAKE_SOURCE_DIR}/sdk/win/include)

# 添加库目录
target_link_directories(${M3U8DOWNLOADER} PRIVATE  ${CMAKE_SOURCE_DIR}/sdk/win/lib)

# 添加链接库
find_package(CURL CONFIG REQUIRED)

target_link_libraries(${M3U8DOWNLOADER} PRIVATE 
	CURL::libcurl 
	polarssl
	findall 
	)

