#ifndef PATH_UTILS_H_
#define PATH_UTILS_H_

/**
 * Convert given relative path to absolute path to file in res/ directory on device.
 *
 * @example if   rel_path = "file.edc"
 *          then abs_path = "/opt/usr/apps/org.example.watch/res/file.edc"
 *          for project with package id "org.example.watch"
 */
void file_full_resource_path_get(const char *relative_path, char *absolute_path);

#endif /* PATH_UTILS_H_ */
