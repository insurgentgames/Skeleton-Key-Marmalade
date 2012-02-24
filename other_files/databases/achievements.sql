CREATE TABLE achievement (num INTEGER,name TEXT,description TEXT,achieved INTEGER);
INSERT INTO achievement (num,name,description,achieved) VALUES('0','Begin the Hunt','Beat the first 10 levels','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('1','Knock Knock','Open a door','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('2','The Doorman','Open 50 doors','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('3','The Door to Nowhere','Can you find it?','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('4','Perfectionist','Play any 10 levels perfectly','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('5','Professional Perfectionist','Play any 30 levels perfectly','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('6','Enter the Darkness','Make it to the Caves','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('7','Enjoy the Sun','Make it to the Beach','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('8','Arrrgh!','Make it to the Pirate Ship','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('9','Treasure Hunter','Beat all level on Easy','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('10','An Adventurer is You','Beat all levels on Medium','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('11','Intrepid Explorer','Beat all levels on Hard','0');
INSERT INTO achievement (num,name,description,achieved) VALUES('12','Mastermind','Beat all levels perfectly','0');

CREATE TABLE achievement_data (doors_opened INTEGER);
INSERT INTO achievement_data (doors_opened) VALUES('0');
