/*
 * Copyright 2013  Alex Merry <alex.merry@kdemail.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#import <Foundation/Foundation.h>

@interface NSString (Util)
	+ (NSString*) stringWithContentsOfFile:(NSString*)path
									 error:(NSError**)error;
	- (id) initWithContentsOfFile:(NSString*)path
							error:(NSError**)error;
@end

// vi:ft=objc:noet:ts=4:sts=4:sw=4
