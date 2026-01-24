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

CREATE TABLE [accounts] (
    [pk]                        INTEGER NOT NULL UNIQUE,
    [fk_user]                   INTEGER NOT NULL,
    [enabled]                   CHAR(1) NOT NULL,       --  'Y' or 'N'
    [emailaddresses]            TEXT,                   --  '\n' - separated, NULL == none
    [login]                     VARCHAR(127) NOT NULL,  --  as per DefaultValidator
    [passwordhash]              CHAR(40) NOT NULL,      --  SHA-1 uppercase hexstring
    [administrator]             CHAR(1) NOT NULL,       --  'Y' or 'N'
    [manageusers]               CHAR(1) NOT NULL,       --  'Y' or 'N'
    [manageactivitytypes]       CHAR(1) NOT NULL,       --  'Y' or 'N'
    [managebeneficiaries]       CHAR(1) NOT NULL,       --  'Y' or 'N'
    [manageworkloads]           CHAR(1) NOT NULL,       --  'Y' or 'N'
    [managepublicactivities]    CHAR(1) NOT NULL,       --  'Y' or 'N'
    [managepublictasks]         CHAR(1) NOT NULL,       --  'Y' or 'N'
    [manageprivateactivities]   CHAR(1) NOT NULL,       --  'Y' or 'N'
    [manageprivatetasks]        CHAR(1) NOT NULL,       --  'Y' or 'N'
    [logwork]                   CHAR(1) NOT NULL,       --  'Y' or 'N'
    [logevents]                 CHAR(1) NOT NULL,       --  'Y' or 'N'
    [generatereports]           CHAR(1) NOT NULL,       --  'Y' or 'N'
    [backupandrestore]          CHAR(1) NOT NULL,       --  'Y' or 'N'
    PRIMARY KEY([pk]),
    FOREIGN KEY([pk]) REFERENCES [objects]([pk]),
    FOREIGN KEY([fk_user]) REFERENCES [users]([pk]),
    CHECK(([enabled] = 'Y') OR ([enabled] = 'N')),
    CHECK(([administrator] = 'Y') OR ([administrator] = 'N')),
    CHECK(([manageusers] = 'Y') OR ([manageusers] = 'N')),
    CHECK(([manageactivitytypes] = 'Y') OR ([manageactivitytypes] = 'N')),
    CHECK(([managebeneficiaries] = 'Y') OR ([managebeneficiaries] = 'N')),
    CHECK(([manageworkloads] = 'Y') OR ([manageworkloads] = 'N')),
    CHECK(([managepublicactivities] = 'Y') OR ([managepublicactivities] = 'N')),
    CHECK(([managepublictasks] = 'Y') OR ([managepublictasks] = 'N')),
    CHECK(([manageprivateactivities] = 'Y') OR ([manageprivateactivities] = 'N')),
    CHECK(([manageprivatetasks] = 'Y') OR ([manageprivatetasks] = 'N')),
    CHECK(([logwork] = 'Y') OR ([logwork] = 'N')),
    CHECK(([logevents] = 'Y') OR ([logevents] = 'N')),
    CHECK(([generatereports] = 'Y') OR ([generatereports] = 'N')),
    CHECK(([backupandrestore] = 'Y') OR ([backupandrestore] = 'N'))
);
CREATE INDEX [idx_account_user] ON [accounts] ([fk_user]);
CREATE INDEX [idx_account_login] ON [accounts] ([login]);
