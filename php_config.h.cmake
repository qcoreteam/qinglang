/* Major version of the PHP++ API */
#define PHP_VERSION_MAJOR ${PHP_VERSION_MAJOR}

/* Minor version of the PHP++ API */
#define PHP_VERSION_MINOR ${PHP_VERSION_MINOR}

/* Patch version of the PHP++ API */
#define PHP_VERSION_PATCH ${PHP_VERSION_PATCH}

/* stage version of the PHP++ API */
#define PHP_VERSION_STAGE "${PHP_VERSION_STAGE}"

/* php++ version string */
#define PHP_VERSION_STR "${PHP_VERSION_STR}"

/* php++ build shared library */
#cmakedefine PHP_BUILD_SHARED_LIBS 1
/* php++ 是否开启异常机制 */
#cmakedefine PHP_ENABLE_EXCEPTIONS 1