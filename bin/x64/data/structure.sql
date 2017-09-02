/*
Navicat SQLite Data Transfer

Source Server         : Chess
Source Server Version : 30808
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30808
File Encoding         : 65001

Date: 2017-08-09 11:36:39
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for rooms
-- ----------------------------
CREATE TABLE IF NOT EXISTS "rooms" (
"name"  TEXT NOT NULL,
"owner"  TEXT NOT NULL,
"user_white"  TEXT,
"user_black"  TEXT,
"private"  INTEGER NOT NULL DEFAULT 0,
"chat_enabled"  INTEGER NOT NULL DEFAULT 1,
"file_name"  TEXT,
PRIMARY KEY ("name" ASC),
CONSTRAINT "user_white_room" FOREIGN KEY ("user_white") REFERENCES "users" ("login") ON DELETE SET NULL ON UPDATE CASCADE,
CONSTRAINT "user_black_room" FOREIGN KEY ("user_black") REFERENCES "users" ("login") ON DELETE SET NULL ON UPDATE CASCADE,
CONSTRAINT "user_owner_room" FOREIGN KEY ("owner") REFERENCES "users" ("login") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for users
-- ----------------------------
CREATE TABLE IF NOT EXISTS "users" (
"login"  TEXT NOT NULL,
"password"  TEXT NOT NULL,
"score"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("login")
);
