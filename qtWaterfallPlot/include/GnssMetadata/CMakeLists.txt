##############################################################
# Copyright(c) 2015 Institute of Navigation
# http://www.ion.org
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
##############################################################

##############################################################
# Add subdirectories
##############################################################
ADD_SUBDIRECTORY(Xml)

##############################################################
# Install information
##############################################################

UHD_INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/AnyUri.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AttributedObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Channel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Comment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DataFile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Date.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Duration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Frame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Frequency.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Location.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Metadata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Oscillator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RfConfiguration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Session.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Stream.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/SubFrame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/System.h 
    
    DESTINATION ${INCLUDE_DIR}/GnssMetadata
    COMPONENT headers
)
