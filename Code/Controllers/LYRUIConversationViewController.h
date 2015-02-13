//
//  LYRUIConversationViewController.h
//  Atlas
//
//  Created by Kevin Coleman on 8/31/14.
//  Copyright (c) 2014 Layer, Inc. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#import <UIKit/UIKit.h>
#import <LayerKit/LayerKit.h>
#import <MapKit/MapKit.h>
#import "LYRUIParticipant.h"
#import "LYRUIMessageInputToolbar.h"
#import "LYRUIAddressBarViewController.h"

@class LYRUIConversationViewController;
@protocol LYRUIMessagePresenting;

///---------------------------------------
/// @name Delegate
///---------------------------------------

@protocol LYRUIConversationViewControllerDelegate <NSObject>

@optional
/**
 @abstract Informs the delegate that a user succesfully sent an `LYRMessage` object.
 @param conversationViewController The `LYRUIConversationViewController` in which the message was sent.
 @param message The `LYRMessage` object that was sent via Layer.
 */
- (void)conversationViewController:(LYRUIConversationViewController *)viewController didSendMessage:(LYRMessage *)message;

/**
 @abstract Informs the delegate that an `LYRMessage` object send attempt failed.
 @param conversationViewController The `LYRUIConversationViewController` in which the message failed to send.
 @param message The `LYRMessage` object which was attempted to be sent via Layer.
 @param error The `NSError` object describing why send failed.
 */
- (void)conversationViewController:(LYRUIConversationViewController *)viewController didFailSendingMessage:(LYRMessage *)message error:(NSError *)error;

/**
 @abstract Informs the delegate that an `LYRMessage` object was tapped.
 @param conversationViewController The `LYRUIConversationViewController` in which the message failed to send.
 @param message The `LYRMessage` object which that was tapped.
 */
- (void)conversationViewController:(LYRUIConversationViewController *)viewController didSelectMessage:(LYRMessage *)message;

/**
 @abstract Asks the delegate for the height to use for a message's cell.
 @param viewController The `LYRUIConversationViewController` where the message cell will appear.
 @param message The `LYRMessage` object that will be displayed in the cell.
 @param cellWidth The width of the message's cell.
 @return The height needed for the message's cell.
 @discussion Applications should only return a value if the `message` object requires a custom cell class. If `nil` is returned, the collection view will default
 to internal height calculations.
 */
- (CGFloat)conversationViewController:(LYRUIConversationViewController *)viewController heightForMessage:(LYRMessage *)message withCellWidth:(CGFloat)cellWidth;

/**
 @abstract Asks the delegate for an `NSOrderedSet` of `LYRMessage` objects representing an `NSArray` of content parts.
 @param viewController The `LYRUIConversationViewController` supplying the content parts.
 @param contentParts The array of content parts supplied via user input into the `messageInputToolbar` property of the controller.
 @return An `NSOrderedSet` of `LYRMessage` objects. If `nil` is returned, the controller will fall back to default behavior. If an empty
 `NSOrderedSet` is returned, the controller will not send any messages.
 @discussion Called when a user taps the `SEND` button on an `LYRUIMessageInputToolbar`. The contentParts array supplied can contain
 either `NSString` or `UIImage` objects. Applications who wish to send `LYRMessage` objects with custom `LYRMessagePart`
 MIME types not supported by default by LayerUIKit can do so by implementing this method. All `LYRMessage` objects returned will be immediately 
 sent into the current conversation for the controller. If implemented, applications should also register custom `UICollectionViewCell` classes 
 with the controller via a call to `registerClass:forMessageCellWithReuseIdentifier:`. They should also implement the optional data source method,
 `conversationViewController:reuseIdentifierForMessage:`.
 */
- (NSOrderedSet *)conversationViewController:(LYRUIConversationViewController *)viewController messagesForContentParts:(NSArray *)contentParts;

@end

///---------------------------------------
/// @name Data Source
///---------------------------------------

@protocol LYRUIConversationViewControllerDataSource <NSObject>

/**
 @abstract Asks the data source for an object conforming to the `LYRUIParticipant` protocol for a given identifier.
 @param conversationViewController The `LYRUIConversationViewController` requesting the object.
 @param participantIdentifer The participant identifier.
 @return An object conforming to the `LYRUIParticpant` protocol.
 */
- (id<LYRUIParticipant>)conversationViewController:(LYRUIConversationViewController *)conversationViewController participantForIdentifier:(NSString *)participantIdentifier;

/**
 @abstract Asks the data source for an `NSAttributedString` representation of a given date.
 @param conversationViewController The `LYRUIConversationViewController` requesting the string.
 @param date The `NSDate` object to be displayed as a string.
 @return an `NSAttributedString` representing the given date.
 @discussion The date string will be displayed above message cells in section headers. The date represents the `sentAt` date of a message object.
 The string can be customized to appear in whatever format your application requires.
 */
- (NSAttributedString *)conversationViewController:(LYRUIConversationViewController *)conversationViewController attributedStringForDisplayOfDate:(NSDate *)date;

/**
 @abstract Asks the data source for an `NSAttributedString` representation of a given `LYRRecipientStatus`.
 @param conversationViewController The `LYRUIConversationViewController` requesting the string.
 @param recipientStatus The `LYRRecipientStatus` object to be displayed as aquestion
 string.
 @return An `NSAttributedString` representing the give recipient status.
 @discussion The recipient status string will be displayed below message the most recent message sent by the authenticated user.
 */
- (NSAttributedString *)conversationViewController:(LYRUIConversationViewController *)conversationViewController attributedStringForDisplayOfRecipientStatus:(NSDictionary *)recipientStatus;

@optional

/**
 @abstract Asks the data source for the collection view cell reuse identifier for a message.
 @param viewController The `LYRUIConversationViewController` requesting the string.
 @param message The `LYRMessage` object to display in the cell.
 @return A string that will be used to dequeue a cell from the collection view.
 @discussion Applications that wish to use custom cells in the `LYRUIConversationViewController` must first register a reuse identifier for their custom cell class.
 This can be done via a call to `registerClass:forMessageCellWithReuseIdentifier:`. Applications should then return the registered reuse identifier only when necessary.
 If `nil` is returned, the collection view will default to internal values for reuse identifiers.
 */
- (NSString *)conversationViewController:(LYRUIConversationViewController *)viewController reuseIdentifierForMessage:(LYRMessage *)message;

/**
 @abstract Asks the data source to provide a conversation for a set of participants.
 @param viewController The `LYRUIConversationViewController` requesting the conversation.
 @param participants A set of objects conforming to `LYRUIParticipant`.
 @return A conversation that will be used by the conversation view controller.
 @discussion Applications may implement this method to override the default behavior which is described below.
 If this method is not implemented or `nil` is returned, the conversation view controller will default to 1) disabling delivery receipts if there are more than five participants and 2) using an existing conversation between the participants if one already exists.
 */
- (LYRConversation *)conversationViewController:(LYRUIConversationViewController *)viewController conversationWithParticipants:(NSSet *)participants;

@end

/**
 @abstract The `LYRUIConversationViewController` class presents an interface that provides for displaying
 a Layer conversation and the ability to send messages. The controller's design and functionality closely correlates with
 the conversation view controller in Messages.
*/
@interface LYRUIConversationViewController : UIViewController <LYRUIAddressBarControllerDelegate>

///---------------------------------------
/// @name Initializing a Controller
///---------------------------------------

/**
 @abstract Creates and returns a new `LYRUIConversationViewController` initialized with an `LYRClient` object.
 @param layerClient The `LYRClient` object from which to retrieve the messages for display.
 @return An `LYRConversationViewController` object.
 */
+ (instancetype)conversationViewControllerWithLayerClient:(LYRClient *)layerClient;

/**
 @abstract The `LYRClient` object used to initialize the controller.
 @discussion If using storyboards, the property must be set explicitly.
 @raises NSInternalInconsistencyException Raised if the value is mutated after the receiver has been presented.
 */
@property (nonatomic) LYRClient *layerClient;

/**
 @abstract The `LYRConversation` object whose messages will be displayed in the controller.
 */
@property (nonatomic) LYRConversation *conversation;

/**
 @abstract The `LYRUIConversationViewControllerDelegate` class informs the reciever to specific events that occured within the controller.
 */
@property (nonatomic, weak) id<LYRUIConversationViewControllerDelegate> delegate;

/**
 @abstract The `LYRUIConversationViewControllerDataSource` class presents an interface allowing
 for the display of information pertaining to specific messages in the conversation view controller
 */
@property (nonatomic, weak) id<LYRUIConversationViewControllerDataSource> dataSource;

/**
 @abstract Register a class for use in creating message collection view cells.
 @param reuseIdentifier The string to be associated with the class.
 */
- (void)registerClass:(Class<LYRUIMessagePresenting>)cellClass forMessageCellWithReuseIdentifier:(NSString *)reuseIdentifier;

/**
 @abstract Returns the `UICollectionViewCell` corresponding to the provided `LYRMessage` object.
 @param message The LYRMessage object used to acquire the cell.
 @return A `UICollectionViewCell` object conforming to the `LYRUIMessagePresenting` protocol.
 @discussion If the provided `LYRMessage` object is not in the current results set of the controller, or the corresponding cell is
 not currently visible, the method may return nil.
 */
- (UICollectionViewCell<LYRUIMessagePresenting> *)collectionViewCellForMessage:(LYRMessage *)message;

///---------------------------------------
/// @name Configuration
///---------------------------------------

/**
 @abstract The time interval at which message dates should be displayed in seconds. Default is 15 minutes meaning that
 dates will appear centered above a message only if the previous message was sent over 15 minutes ago.
 */
@property (nonatomic) NSTimeInterval dateDisplayTimeInterval;

/**
 @abstract Informs the receiver if it should marks messages as read.
 @discussion If `YES`, the controller will mark all messages in the conversation as read when it comes on screen.
 @default `YES`.
 */
@property (nonatomic) BOOL marksMessagesAsRead;

/**
 @abstract Informs the receiver if it should display a `LYRUIAddressBarController`. If yes, your application must implement
 `LYRUIAddressBarControllerDelegate` and `LYRUIAddressBarControllerDataSource`. Default is no.
 */
@property (nonatomic) BOOL displaysAddressBar;

///---------------------------------------
/// @name Public Accessors
///---------------------------------------

/**
 @abstract The `LYRUIAddressBarViewController` displayed for addressing new conversations.
 */
@property (nonatomic) LYRUIAddressBarViewController *addressBarController;

/**
 @abstract The `LYRUIMessageInputToolbar` displayed for user input.
 */
@property (nonatomic) LYRUIMessageInputToolbar *messageInputToolbar;



@end
