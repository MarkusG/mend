# mend

mend is a notetaking application that utilizes a relational database.

## Building

To build mend, install meson and your distribution's libpq development packages, and then run
```
$ meson build && ninja -C build
```

## Testing

There are two options for testing: running a mend database locally and using Docker. To run a local database, install and start PostgreSQL and initialize your database cluster. Then, create the mend database and run the tests:
```
$ createdb mend
$ psql -f sql/tables.sql
$ MEND_CONNECTION_STRING="dbname=mend" ninja -C build test
```

To use Docker, simply install Docker and docker-compose and run
```
$ docker-compose up --exit-code-from mend --build mend
```

This has the downside of leaving the PostgreSQL container running after the tests have completed. If this is an issue, run `docker-compose down` to kill the PostgreSQL container.
