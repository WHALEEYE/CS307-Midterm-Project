import json
import re


def replace(matched):
  value = str(matched.group('inner'))
  return '（' + value + '）'


def iden(matched):
  value = str(matched.group('lev'))
  return value


with open("course_info.json", "r", encoding='utf-8') as f1:
  data = json.load(f1)
  for i in data:
    if not re.match("[A-Za-z\\s]*\\Z", str(i['courseName'])):  # 非纯英文课程名字段处理
      i['courseName'] = re.sub("\\((?P<inner>.)\\)", replace,
                               # 课程名中的英文括号用中文括号替换
                               str(i['courseName']))
      i['courseName'] = re.sub("\\A\\s", "", str(i['courseName']))  # 去除开头的空格
      i['courseName'] = re.sub("\\s(?P<lev>[ABCDI])", iden,
                               str(i['courseName']))  # 去除课程名和课程等级之间的空格
    if not re.match("[A-Za-z\\s]*\\Z",
                    str(i['prerequisite'])):  # 先修课非纯英文课程名字段处理
      i['prerequisite'] = re.sub("\\((?P<inner>.)\\)", replace,
                                 str(i['prerequisite']))  # 先修课课程名中的英文括号用中文括号替换
      i['prerequisite'] = re.sub("\\A\\s", "",
                                 str(i['prerequisite']))  # 去除开头的空格
      i['prerequisite'] = re.sub("\\s(?P<lev>[ABCDI])", iden,
                                 str(i['prerequisite']))  # 去除课程名和课程等级之间的空格
      i['prerequisite'] = re.sub("\\(", "(|",
                                 re.sub("\\)", "|)",
                                        i['prerequisite']))  # 在上下括号和表达式之间加上分隔符
      i['prerequisite'] = re.sub("\\s+", "|", i['prerequisite'])  # 将分隔符从空格更换为|
    i['className'] = re.sub("\\s+", "",
                            str(i['className']))  # 去除className字段中的空白符
    i['teacher'] = re.sub("\\A\\s", "", str(i['teacher']))  # 去除teacher字段中开头的空白符
    for j in i['classList']:
      j['location'] = re.sub("\\s+", "",
                             str(j['location']))  # 去除location字段中的空格符
with open("output.json", "w", encoding="utf-8") as f2:
  f2.write(json.dumps(data).encode('utf-8').decode('unicode_escape'))
