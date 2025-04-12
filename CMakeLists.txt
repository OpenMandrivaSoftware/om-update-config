cmake_minimum_required(VERSION 4.0)
project(om-update-config CXX)

add_subdirectory(src)

install(PROGRAMS om-update-config DESTINATION bin)
install(PROGRAMS update-system DESTINATION bin)

install(FILES system-update.service DESTINATION lib/systemd/system)
install(FILES system-update.timer DESTINATION lib/systemd/system)
install(FILES system-update.logrotate DESTINATION /etc/logrotate.d)
