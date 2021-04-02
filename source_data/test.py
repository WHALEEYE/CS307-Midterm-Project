import json

with open("course_info.json", "r", encoding="utf-8") as f:
  data = json.load(f)
  for i in data:
    # for j in i['classList']:
    #   print('|' + j['location'] + '|')
    print(i['prerequisite'])