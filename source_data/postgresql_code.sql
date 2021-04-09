create table if not exists course
(
    Id            serial primary key,
    courseId      varchar(10) unique not null,
    courseCredit  int                not null,
    totalCapacity int                not null,
    courseHour    int                not null,
    courseName    varchar            not null,
    courseDept    varchar            not null,
    constraint unique_course unique (courseName, courseDept)
);

create table if not exists class
(
    Id        serial primary key,
    courseId  integer,
    className varchar not null,
    teacher   varchar,

    constraint fk_courseId foreign key (courseId) references course (Id)
);

create table if not exists classList
(
    Id        serial primary key,
    classId   integer,
    weekList  varchar[] not null,
    location  varchar,
    classTime varchar   not null,
    weekDay   integer   not null,
    constraint fk_courseId foreign key (classId) references class (Id)
);

create table if not exists prerequisite
(
    Id                serial primary key,
    courseId          integer unique,
    requisiteCourseId varchar,
    constraint fk_courseId foreign key (courseId) references course (Id)
);

create table if not exists teacher
(
    Id      serial primary key,
    classId integer unique,
    name    varchar,
    constraint fk_courseId foreign key (classId) references class (Id)
);

create table if not exists student
(
    Id         serial primary key,
    name       varchar,
    sex        varchar,
    department varchar,
    studentId  int
);

delete
from course;
delete
from class;
delete
from prerequisite;
TRUNCATE TABLE course RESTART IDENTITY CASCADE;
TRUNCATE TABLE class RESTART IDENTITY CASCADE;
TRUNCATE TABLE prerequisite RESTART IDENTITY CASCADE;

select *
from course
where courseName = '毕业论文（设计）';
alter table course
    add constraint unique_course unique (courseName, courseDept);

alter table course
    drop constraint unique_course;

select *
from course
where courseName like '%化学原理%';