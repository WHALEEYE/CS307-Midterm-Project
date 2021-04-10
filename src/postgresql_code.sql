create table if not exists course
(
    Id            int                primary key,
    courseId      varchar(10) unique not null,
    courseCredit  int                not null,
    totalCapacity int                not null,
    courseHour    int                not null,
    courseName    varchar            not null,
    courseDept    varchar            not null,
    requisiteCourseId varchar
);

drop table course;

create table if not exists class
(
    Id int primary key ,
    courseId integer,
    className varchar not null,

    constraint fk_courseId foreign key (courseId) references course(Id)
);

drop table class;

create table if not exists classList
(
    Id serial primary key ,
    classId integer,
    weekList varchar not null,
    location varchar,
    classTime varchar not null ,
    weekDay integer not null ,
    constraint fk_courseId foreign key (classId) references class(Id)
);

drop table classList;


create table if not exists prerequisite
(
    Id serial primary key ,
    courseId integer unique,
    requisiteCourseId varchar,
    constraint fk_courseId foreign key (courseId) references course(Id)
);

drop table prerequisite;

create table if not exists teacher
(
    Id serial primary key ,
    classId integer unique ,
    name varchar,
    constraint fk_courseId foreign key (classId) references class(Id)
);

drop table teacher;

create table if not exists student
(
    Id int primary key ,
    name varchar,
    sex varchar,
    department varchar,
    studentId int
);

drop table student;

create table if not exists student_class
(
    Id serial primary key ,
    studentId int not null ,
    courseId varchar(10),
    constraint fk_studentId foreign key (studentId) references student(Id)
);

drop table student_class;

