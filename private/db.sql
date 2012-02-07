# ****************************************************************
# categories
# ****************************************************************
CREATE TABLE categories (
  Id             int,
  Category       varchar(64) NOT NULL,
  PRIMARY KEY (Id)
) TYPE=InnoDB;


# ****************************************************************
# levels
# ****************************************************************
CREATE TABLE levels (
  Id             int,
  Level          varchar(64) NOT NULL,
  PRIMARY KEY (Id)
) TYPE=InnoDB;


# ****************************************************************
# words
# ****************************************************************
CREATE TABLE words (
  Id             int NOT NULL AUTO_INCREMENT,
  Word           varchar(64) NOT NULL,
  Category_Id    int,
  Level_Id       int,
  Audio_File     varchar(64) NOT NULL,
  INDEX (Category_Id),
  FOREIGN KEY (Category_Id) REFERENCES categories(Id) ON DELETE CASCADE,
  INDEX (Level_Id),
  FOREIGN KEY (Level_Id) REFERENCES levels(Id) ON DELETE CASCADE,
  PRIMARY KEY (Id)
) TYPE=InnoDB;


# ****************************************************************
# users
# ****************************************************************
CREATE TABLE users (
  Id             int NOT NULL AUTO_INCREMENT,
  User           varchar(64) NOT NULL,
  Password       varchar(64) NOT NULL,
  PRIMARY KEY (Id),
  UNIQUE (User)
) TYPE=InnoDB;


# ****************************************************************
# results
# ****************************************************************
CREATE TABLE results (
  User_Id int,
  Word_Id int,
  Tries int,
  Correct int,
  Streak int,
  INDEX(User_Id),
  FOREIGN KEY (User_Id) REFERENCES users(Id) ON DELETE CASCADE,
  INDEX(Word_Id),
  FOREIGN KEY (Word_Id) REFERENCES words(Id) ON DELETE CASCADE,
  UNIQUE (User_Id, Word_Id)
) TYPE=InnoDB;

# ****************************************************************
# userList
# ****************************************************************
CREATE TABLE userList (
  User_Id int,
  Word_Id int,
  UNIQUE (User_Id, Word_Id),
  FOREIGN KEY (User_Id) REFERENCES users(Id) ON DELETE CASCADE,
  FOREIGN KEY (Word_Id) REFERENCES words(Id) ON DELETE CASCADE
) TYPE=InnoDB;

# ****************************************************************
# bookmarks
# ****************************************************************
CREATE TABLE bookmarks (
  User_Id int,
  Word_Id int,
  Type enum('booklist', 'userlist'),
  INDEX(User_Id),
  FOREIGN KEY (User_Id) REFERENCES users(Id) ON DELETE CASCADE,
  INDEX(Word_Id),
  FOREIGN KEY (Word_Id) REFERENCES words(Id) ON DELETE CASCADE,
  UNIQUE (User_Id)
) TYPE=InnoDB;
