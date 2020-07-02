CREATE EXTENSION IF NOT EXISTS "uuid-ossp" WITH SCHEMA public;

CREATE OR REPLACE FUNCTION trigger_update()
RETURNS trigger AS $$
BEGIN
	NEW.updated = now();
	RETURN NEW;
END
$$ LANGUAGE plpgsql;

CREATE TABLE IF NOT EXISTS entity (
	uid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
	created TIMESTAMP NOT NULL DEFAULT now()
);

CREATE TABLE IF NOT EXISTS alias (
	uid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
	entity UUID NOT NULL REFERENCES entity(uid) ON DELETE CASCADE,
	value TEXT NOT NULL,
	precedence INTEGER,
	since TIMESTAMP NOT NULL DEFAULT now()
);

CREATE TABLE IF NOT EXISTS note (
	uid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
	entity UUID NOT NULL REFERENCES entity(uid) ON DELETE CASCADE,
	value TEXT NOT NULL,
	created TIMESTAMP NOT NULL DEFAULT now(),
	updated TIMESTAMP NOT NULL DEFAULT now()
);

CREATE TRIGGER set_timestamp
BEFORE UPDATE ON note
FOR EACH ROW
	EXECUTE PROCEDURE trigger_update();

CREATE TABLE IF NOT EXISTS relation (
	uid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
	entity_a UUID NOT NULL REFERENCES entity(uid) ON DELETE CASCADE,
	entity_b UUID NOT NULL REFERENCES entity(uid) ON DELETE CASCADE,
	note TEXT,
	created TIMESTAMP NOT NULL DEFAULT now(),
	updated TIMESTAMP NOT NULL DEFAULT now()
);

CREATE TRIGGER set_timestamp
BEFORE UPDATE ON relation
FOR EACH ROW
	EXECUTE PROCEDURE trigger_update();

CREATE VIEW primary_alias AS
SELECT DISTINCT ON (entity) entity, value AS alias
FROM
(
	SELECT * FROM alias
	ORDER BY precedence
) AS tbl;
