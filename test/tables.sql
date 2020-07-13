INSERT INTO entity (uid, name, created) VALUES (UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'Foo', TIMESTAMP WITHOUT TIME ZONE '2001-09-28 23:00');
INSERT INTO entity (uid, name, created) VALUES (UUID 'bc05cd9e-098a-42c6-a92f-21f4529a9f89', 'Bar', TIMESTAMP WITHOUT TIME ZONE '2001-10-28 23:00');
INSERT INTO entity (uid, name, created) VALUES (UUID '17e3f02a-8e2d-4a66-abca-d1082a9f9508', 'Baz', TIMESTAMP WITHOUT TIME ZONE '2001-11-28 23:00');
INSERT INTO entity (uid, name) VALUES (UUID '70358375-01b7-4f87-b477-3711fbacedc6', 'to_be_removed');
INSERT INTO entity (uid, name) VALUES (UUID 'ddbfba06-b10c-4344-a343-ccc63321d435', 'to_be_removed_by_uuid');

INSERT INTO alias (uid, entity, value, since) VALUES (UUID 'dd4e8f06-0124-4945-a58f-5d4931263799', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'Foo alias', TIMESTAMP WITHOUT TIME ZONE '2001-11-28 23:00');
INSERT INTO alias (uid, entity, value) VALUES (UUID '3fcd6a93-0a4c-4241-ac56-3be36ca02ef8', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'to_be_removed');
INSERT INTO alias (uid, entity, value) VALUES (UUID '53defebb-efbb-40ba-bbe0-0b1105a474b1', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'to_be_removed_by_value');

INSERT INTO note (uid, entity, value, created, updated) VALUES (UUID '01a04e4b-0a7e-4aae-ba19-0a5500c35b66', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'Foo note', TIMESTAMP WITHOUT TIME ZONE '2001-10-28 23:00', TIMESTAMP WITHOUT TIME ZONE '2001-11-28 23:00');
INSERT INTO note (uid, entity, value, created, updated) VALUES (UUID '5f6d52a9-20f6-4dad-aa29-76ee6e097622', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'to_be_edited', TIMESTAMP WITHOUT TIME ZONE '2001-10-28 23:00', TIMESTAMP WITHOUT TIME ZONE '2001-11-28 23:00');
INSERT INTO note (uid, entity, value) VALUES (UUID 'c97bc7a8-7a3a-4038-ad5f-11bfc875b8d7', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'to_be_removed');
