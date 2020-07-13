INSERT INTO entity (uid, name, created) VALUES (UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'Foo', TIMESTAMP WITHOUT TIME ZONE '2001-09-28 23:00');
INSERT INTO entity (uid, name, created) VALUES (UUID 'bc05cd9e-098a-42c6-a92f-21f4529a9f89', 'Bar', TIMESTAMP WITHOUT TIME ZONE '2001-10-28 23:00');
INSERT INTO entity (uid, name, created) VALUES (UUID '17e3f02a-8e2d-4a66-abca-d1082a9f9508', 'Baz', TIMESTAMP WITHOUT TIME ZONE '2001-11-28 23:00');
INSERT INTO entity (uid, name) VALUES (UUID '70358375-01b7-4f87-b477-3711fbacedc6', 'to_be_removed');
INSERT INTO entity (uid, name) VALUES (UUID 'ddbfba06-b10c-4344-a343-ccc63321d435', 'to_be_removed_by_uuid');

INSERT INTO alias (uid, entity, value, since) VALUES (UUID 'dd4e8f06-0124-4945-a58f-5d4931263799', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'Foo alias', TIMESTAMP WITHOUT TIME ZONE '2001-11-28 23:00');
INSERT INTO alias (uid, entity, value) VALUES (UUID '3fcd6a93-0a4c-4241-ac56-3be36ca02ef8', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'to_be_removed');
INSERT INTO alias (uid, entity, value) VALUES (UUID '53defebb-efbb-40ba-bbe0-0b1105a474b1', UUID 'a09d03f1-9e17-4127-b22c-08fbe65ef07f', 'to_be_removed_by_value');
