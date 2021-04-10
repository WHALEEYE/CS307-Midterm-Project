--需要更新
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

create table if not exists teacher
(
    Id      serial primary key,
    classId integer unique,
    name    varchar,
    constraint fk_courseId foreign key (classId) references class (Id)
);

create table if not exists department
(
    id           serial primary key,
    chinese_name varchar(20) unique not null,
    english_name varchar(40) unique
);

create table if not exists student
(
    id             serial primary key,
    name           varchar(10) not null,
    sex            char(1)     not null,
    department_id  int,
    student_number int         not null unique,
    constraint dep_for_key foreign key (department_id) references department (id)
);

create table if not exists course_selection
(
    id         serial primary key,
    student_id int not null,
    course_id  int not null,
    constraint stu_for_key foreign key (student_id) references student (id),
    constraint cor_for_key foreign key (course_id) references course (id)
);

insert into department (chinese_name, english_name)
values ('阿兹卡班', 'Azkaban');
insert into department (chinese_name, english_name)
values ('斯莱特林', 'Slytherin');
insert into department (chinese_name, english_name)
values ('赫奇帕奇', 'Hufflepuff');
insert into department (chinese_name, english_name)
values ('拉文克劳', 'Ravenclaw');
insert into department (chinese_name, english_name)
values ('格兰芬多', 'Gryffindor');

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