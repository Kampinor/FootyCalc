--03.08.2023 

--Adden  Liga
Insert into Leagues values (0,'L','Bundesliga');

--Adden Saison
Insert into Season values (0,'2022/2023',0);


--04.08.2023
--Adden Playgrounds
Insert into Playground values (0,'Deutsche Bank Park'); --FRA
Insert into Playground values (1,'Allianz Arena'); --FCB
Insert into Playground values (2,'Signal Iduna Park'); --BVB
Insert into Playground values (3,'Red Bull Arena'); --RBL
Insert into Playground values (4,'Stadion an der Alten Försterei'); --UNI
Insert into Playground values (5,'Europa-Park Stadion'); --FRE
Insert into Playground values (6,'BayArena'); --LEV
Insert into Playground values (7,'Volkswagen Arena'); --WOL
Insert into Playground values (8,'MEWA Arena'); --MAI
Insert into Playground values (9,'Rhein-Energie Stadion'); --FCK
Insert into Playground values (10,'Borussia-Park'); --BMG
Insert into Playground values (11,'PreZero Arena'); --HOF
Insert into Playground values (12,'MHPArena'); --STU
Insert into Playground values (13,'Weserstadion'); -- Wer
Insert into Playground values (14,'Vonovia Ruhrstadion'); -- Boc
Insert into Playground values (15,'WWK Arena'); -- fca
Insert into Playground values (16,'Olympiastadion Berlin'); -- her
Insert into Playground values (17,'VELTINS-Arena'); -- fcs
Insert into Playground values (18,'Merck-Stadion am Böllenfalltor'); -- svd
Insert into Playground values (19,'Voith-Arena'); -- Hei

--07/08.08.2023
--Adden Teams
Insert into Teams values (0,0,'Eintracht Frankfurt');
Insert into Teams values (1,1,'FC Bayern München');
Insert into Teams values (2,2,'Borussia Dortmund');
Insert into Teams values (3,3,'RB Leipzig');
Insert into Teams values (4,4,'Union Berlin');
Insert into Teams values (5,5,'SC Freiburg');
Insert into Teams values (6,6,'Bayer Leverkusen');
Insert into Teams values (7,7,'VFL Wolfsburg');
Insert into Teams values (8,8,'FSV Mainz 05');
Insert into Teams values (9,9,'FC Köln');
Insert into Teams values (10,10,'Borussia MGladbach');
Insert into Teams values (11,11,'TSG Hoffenheim');
Insert into Teams values (12,12,'VFB Stuttgart');
Insert into Teams values (13,13,'Werder Bremen');
Insert into Teams values (14,14,'VFL Bochum');
Insert into Teams values (15,15,'FC Augsburg');
Insert into Teams values (16,16,'Hertha BSC');
Insert into Teams values (17,17,'FC Schalke 04');
Insert into Teams values (18,18,'SV Darmstadt');
Insert into Teams values (19,19,'FC Heidenheim');

-- Adden Coaches 
Insert into Coaches values (0,'Dino Toppmöller',0);
Insert into Coaches values (1,'Thomas Tuchel',1);
Insert into Coaches values (2,'Edin Terzić',2);
Insert into Coaches values (3,'Marco Rose',3); 
Insert into Coaches values (4,'Urs Fischer',4); 
Insert into Coaches values (5,'Christian Streich',5);
Insert into Coaches values (6,'Xabi Alonso',6); 
Insert into Coaches values (7,'Niko Kovač',7); 
Insert into Coaches values (8,'Bo Svensson',8); 
Insert into Coaches values (9,'Steffen Baumgart',9); 
Insert into Coaches values (10,'Gerardo Seoane',10); 
Insert into Coaches values (11,'Sebastian Hoeneß',11); 
Insert into Coaches values (12,'Michael Wimmer',12); 
Insert into Coaches values (13,'Ole Werner',13); 
Insert into Coaches values (14,'Thomas Letsch',14); 
Insert into Coaches values (15,'Markus Weinzierl',15); 
Insert into Coaches values (16,'Pal Dardai',16); 
Insert into Coaches values (17,'Thomas Reis',17); 
Insert into Coaches values (18,'Thorsten Lieberknecht',18); 
Insert into Coaches values (19,'Frank Schmidt',19);

--18.08.2023
ALTER Table Results Add Matchday int;

--11.09.2023
alter table Results add column RedCards_H int;
alter table Results add column RedCards_A int;

--15.09.2023
--Views für Schnitt
Create View Heimstatistik as
SELECT t.TName,AVG(r.Tore_H) as "Heimtore",AVG(r.Tore_A) as "Heimgegentore",AVG(r.xG_H) as "HeimXG",AVG(r.xG_A) as "HeimXGG", COUNT(r.Team_H) as Games
from Results r 
left join Teams t on t.Team_ID = r.Team_H 
where r.Matchday < 8
GROUP by t.Team_ID 
;

Create View Heimstatistik_o_Rot as
SELECT t.TName,AVG(r.Tore_H) as "Heimtore",AVG(r.Tore_A) as "Heimgegentore",AVG(r.xG_H) as "HeimXG",AVG(r.xG_A) as "HeimXGG", COUNT(r.Team_H) as Games
from Results r 
left join Teams t on t.Team_ID = r.Team_H 
where r.Matchday < 8 and r.RedCards_H =0 and r.RedCards_A =0
GROUP by t.Team_ID 
;

Create View Auswartsstatistik as
SELECT t.TName,AVG(r.Tore_A) as "Auswärtstore",AVG(r.Tore_H) as "Auswärtsgegentore",AVG(r.xG_A) as "AusXG",AVG(r.xG_H) as "AusXGG", COUNT(r.Team_H) as Games
from Results r 
inner join Teams t on t.Team_ID = r.Team_A 
where r.Matchday < 8
GROUP by t.Team_ID 
;

Create View Auswartsstatistik_o_Rot as
SELECT t.TName,AVG(r.Tore_A) as "Auswärtstore",AVG(r.Tore_H) as "Auswärtsgegentore",AVG(r.xG_A) as "AusXG",AVG(r.xG_H) as "AusXGG", COUNT(r.Team_H) as Games
from Results r 
inner join Teams t on t.Team_ID = r.Team_A 
where r.Matchday < 8 and r.RedCards_H =0 and r.RedCards_A =0
GROUP by t.Team_ID 
;