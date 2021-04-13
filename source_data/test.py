import json
import re

with open("course_info.json", "r", encoding='utf-8') as f1:
  data = json.load(f1)
  for i in data:
    print(i['prerequisite'])