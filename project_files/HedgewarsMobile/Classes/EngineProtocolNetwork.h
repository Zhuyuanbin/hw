/*
 * Hedgewars-iOS, a Hedgewars port for iOS devices
 * Copyright (c) 2009-2012 Vittorio Giovara <vittorio.giovara@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */


#import <Foundation/Foundation.h>
#import "SDL_net.h"


@protocol EngineProtocolDelegate <NSObject>

- (void)gameEndedWithStatistics:(NSArray *)stats;

@end


@interface EngineProtocolNetwork : NSObject {
    id<EngineProtocolDelegate> __weak delegate;
    NSOutputStream *stream;
    TCPsocket csd;
    NSInteger enginePort;
}

@property (nonatomic, weak) id<EngineProtocolDelegate> delegate;
@property (nonatomic, strong) NSOutputStream *stream;
@property (assign) TCPsocket csd;
@property (assign) NSInteger enginePort;

- (id)  init;
- (id)  initWithPort:(NSInteger)port;
- (void)spawnThread:(NSString *)onSaveFile withOptions:(NSDictionary *)dictionary;
- (void)engineProtocol:(id)object;

-(int)  sendToEngine:(NSString *)string;
-(int)  sendToEngineNoSave:(NSString *)string;
- (void)provideTeamData:(NSString *)teamName forHogs:(NSInteger)numberOfPlayingHogs withHealth:(NSInteger)initialHealth ofColor:(NSNumber *)teamColor;
- (void)provideAmmoData:(NSString *)ammostoreName forPlayingTeams:(NSInteger)numberOfTeams;
- (NSInteger)provideScheme:(NSString *)schemeName;

@end
