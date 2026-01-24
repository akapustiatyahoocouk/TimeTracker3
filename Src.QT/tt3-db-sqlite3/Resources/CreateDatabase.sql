CREATE TABLE [objects] (
    [pk]    INTEGER NOT NULL UNIQUE,
    [oid]   CHAR(38) NOT NULL UNIQUE,   --  {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
    [type]  VARCHAR(32) NOT NULL,       --  "User", "Account", etc.
    PRIMARY KEY([pk] AUTOINCREMENT),
    UNIQUE("oid")
);
