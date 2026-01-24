CREATE TABLE [objects] (
    [pk]    INTEGER NOT NULL UNIQUE,    --  auto-generated
    [oid]   CHAR(38) NOT NULL UNIQUE,   --  {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
    [type]  VARCHAR(32) NOT NULL,       --  'User', 'Account', etc.
    PRIMARY KEY([pk] AUTOINCREMENT),
    UNIQUE([oid]),
    CHECK(([type] = 'User') OR
          ([type] = 'Account'))
);

CREATE TABLE [users] (
    [pk]                INTEGER NOT NULL UNIQUE,
    [enabled]           CHAR(1) NOT NULL,   --  'Y' or 'N'
    [emailaddresses]    TEXT,          --  '\n' - separated, NULL == none
    [realname]          VARCHAR(127) NOT NULL,  --  as per DefaultValidator
    [inactivitytimeout] VARCHAR(12),--  'PThhhhhHmmM', NULL == absent
    [uilocale]          VARCHAR(5),          --  'll_CC', NULL == absent
    PRIMARY KEY([pk]),
    FOREIGN KEY([pk]) REFERENCES [objects]([pk]),
    CHECK(([enabled] = 'Y') OR ([enabled] = 'N'))
);

