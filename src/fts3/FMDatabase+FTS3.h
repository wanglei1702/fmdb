//
//  FMDatabase+FTS3.h
//  fmdb
//
//  Created by Andrew on 3/27/14.
//  Copyright (c) 2014 Andrew Goodale. All rights reserved.
//

#import "FMDatabase.h"

/**
 Names of commands that can be issued against an FTS table.
 */
extern NSString *const kFTSCommandOptimize;        // "optimize"
extern NSString *const kFTSCommandRebuild;         // "rebuild"
extern NSString *const kFTSCommandIntegrityCheck;  // "integrity-check"
extern NSString *const kFTSCommandMerge;           // "merge=%u,%u"
extern NSString *const kFTSCommandAutoMerge;       // "automerge=%u"

@protocol FMTokenizerDelegate;

/**
  This category provides methods to access the FTS3 extensions in SQLite.
 */
@interface FMDatabase (FTS3)

/**
 Register a delgate implementation in the global table. The name should be used
 as a parameter when creating the table.
 */
+ (void)registerTokenizer:(id<FMTokenizerDelegate>)tokenizer withName:(NSString *)name;

/**
 Calls the `fts3_tokenizer()` function on this database, installing the "fmdb" tokenizer module.
 */
- (BOOL)installTokenizerModule;

/**
 Runs a "special command" for FTS3/FTS4 tables.
 */
- (BOOL)issueCommand:(NSString *)command forTable:(NSString *)tableName;

@end

#pragma mark

/* Extend this structure with your own custom cursor data */
typedef struct FMTokenizerCursor
{
    void       *tokenizer;      /* Internal SQLite reference */
    void       *tempBuffer;     /* Internal temporary memory */
    CFStringRef inputString;    /* The input text being tokenized */
    CFRange     currentRange;   /* The current offset within `inputString` */
    CFStringRef tokenString;    /* The contents of the current token */
    CFTypeRef   userObject;     /* Additional state for the cursor */
    int         tokenIndex;     /* Index of next token to be returned */
} FMTokenizerCursor;

@protocol FMTokenizerDelegate

- (void)openTokenizerCursor:(FMTokenizerCursor *)cursor;

- (BOOL)nextTokenForCursor:(FMTokenizerCursor *)cursor;

- (void)closeTokenizerCursor:(FMTokenizerCursor *)cursor;

@end