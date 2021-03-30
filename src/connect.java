import com.google.gson.JsonArray;
import com.google.gson.JsonIOException;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonSyntaxException;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class connect {

  public static void main(String[] args) throws Exception {
    // 参数：
    // jdbc协议:postgresql子协议://主机地址:数据库端口号/要连接的数据库名
    String url = "jdbc:postgresql://121.4.163.50:10943/test_fei";
    // 数据库用户名
    String user = "postgres";
    // 数据库密码
    String password = "What1sth1s";

    // 1. 加载Driver类，Driver类对象将自动被注册到DriverManager类中
    java.lang.Class.forName("org.postgresql.Driver");
    // 2. 连接数据库，返回连接对象
    Connection conn = DriverManager.getConnection(url, user, password);

    try {
      //新建解析json文件的模块
      String dataPath = "./source_data/course_info.json";//配置解析的文本路径
      JsonParser parser = new JsonParser();  //创建JSON解析器
      BufferedReader in = new BufferedReader(
          new InputStreamReader(new FileInputStream(dataPath), StandardCharsets.UTF_8),
          50 * 1024 * 1024); //设置缓冲区 编码
      JsonArray jsonArray = (JsonArray) parser.parse(in);  //创建JsonArray对象

      //新建解析csv的文件模块
      BufferedReader br = new BufferedReader(
          new InputStreamReader(
              new FileInputStream("./source_data/select_course.csv")
          )
      );

      //程序化对数据库输入数据:  (int::n = 100只在测试阶段使用,后期换成json文件的长度)
      int n = jsonArray.size();
      // insert_Course(jsonArray, conn, n);  //course表中数据的输入
      // insert_Class(jsonArray, conn, n);   //class表中数据的输入
      // insert_pre(jsonArray, conn, n);     //先修课输入 转换成数字及符号
      // insert_teacher(jsonArray, conn, n); //teacher表中数据的输入
      // insert_student(br, conn, n);

    } catch (JsonIOException | FileNotFoundException | JsonSyntaxException e) {
      e.printStackTrace();
    }
  }

  //course数据的输入封装
  private static void insert_Course(JsonArray jsonArray, Connection conn, int n)
      throws SQLException {
    for (int i = 0; i < n; i++) {
      //从json文件中读取对应序号的信息，创造course_info对象，准备后续输入数据库中
      course_info course_info = new course_info();
      jsonToCourse(jsonArray, i, course_info);
      //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
      String sql_addCourseIfNotExist =
          "INSERT INTO course(id, totalCapacity, courseId, courseHour, courseCredit, courseName, courseDept) VALUES(DEFAULT,?,?,?,?,?,?)"
              + "ON conflict(courseId)  DO NOTHING;";
      PreparedStatement ps_addCourse = conn
          .prepareStatement(sql_addCourseIfNotExist, Statement.RETURN_GENERATED_KEYS);
      //将course_info对象中的信息导入数据库中
      courseToDatabase(ps_addCourse, course_info);
    }
  }

  //class数据的输入封装
  private static void insert_Class(JsonArray jsonArray, Connection conn, int n)
      throws SQLException {
    for (int i = 0; i < n; i++) {
      //构造查询，辅助class_info对象数据输入
      String sql_get_courseId = "select id from course where courseName = ?";
      PreparedStatement ps_get_CourseId = conn.prepareStatement(sql_get_courseId);
      //从json文件中读取对应序号的信息，创造class_info对象，准备后续输入数据库中
      class_info class_info = new class_info();
      jsonToClass(jsonArray, ps_get_CourseId, i, class_info);
      //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
      String sql_add_Class = "INSERT INTO class(id, courseId, className) VALUES(DEFAULT, ?, ?)";
      PreparedStatement ps_add_class = conn.prepareStatement(sql_add_Class);
      //将class_info对象中的信息导入数据库中
      classToDatabase(ps_add_class, class_info);
    }
  }

  //teacher数据的输入封装
  private static void insert_teacher(JsonArray jsonArray, Connection conn, int n)
      throws SQLException {
    for (int i = 0; i < n; i++) {
      //构造查询，辅助teacher对象数据输入
      String sql_get_classId = "select id from class where className = ?";
      PreparedStatement ps_get_classId = conn.prepareStatement(sql_get_classId);
      //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
      String sql_add_Teacher = "INSERT INTO teacher(id, classId, name) VALUES(DEFAULT,?,?)"
          + "ON conflict(classId) DO NOTHING;";
      PreparedStatement ps_addTeacher = conn.prepareStatement(sql_add_Teacher);
      //从json文件中读取对应序号的信息，创造teacher对象，同时输入数据库中
      jsonToTeacher(jsonArray, ps_get_classId, i, ps_addTeacher);
    }
  }

  //prerequisite数据的输入封装
  private static void insert_pre(JsonArray jsonArray, Connection conn, int n) throws SQLException {
    for (int i = 0; i < n; i++) {
      prerequisite prerequisite = new prerequisite();
      //构造查询 根据courseName查询course的id
      String sql_get_courseId = "select id from course where courseName = ?";
      PreparedStatement ps_get_CourseId = conn.prepareStatement(sql_get_courseId);
      jsonToPre(jsonArray, ps_get_CourseId, i, prerequisite);
      //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
      String sql_add_Pre =
          "INSERT INTO prerequisite(id, courseId, requisiteCourseId) VALUES(DEFAULT, ?, ?)"
              + "ON conflict(courseId)  DO NOTHING;";
      PreparedStatement ps_add_Pre = conn.prepareStatement(sql_add_Pre);
      //将prerequisite对象中的信息导入数据库中（待完成）
      preToDatabase(ps_add_Pre, prerequisite);
    }
  }

  //student数据的输入封装
  private static void insert_student(BufferedReader br, Connection conn, int n)
      throws IOException, SQLException {
    //准备导入数据库所需的jdbc::PreparedStatement, 以及对应的sql语句
    String sql_add_student = "INSERT INTO student(id, name, sex, department, studentId) VALUES(DEFAULT,?,?,?,?)";
    PreparedStatement ps_addStudent = conn.prepareStatement(sql_add_student);
    String new_line;
    while ((new_line = br.readLine()) != null) {
      student student = new student();
      String[] info = new_line.split(",");
      student.setName(info[0].trim());
      student.setSex(info[1].trim());
      student.setDepartment(info[2].trim());
      student.setStudentId(Integer.parseInt(info[3].trim()));
      studentToDatabase(ps_addStudent, student);
    }
  }

  //以下是几个所用到的最基础组件：
  //将json文件中对应序号的数据读取并赋值给Java对象的组件
  private static void jsonToCourse(JsonArray jsonArray, int index, course_info course_info) {
    JsonObject jsonOBJ_courseArray = jsonArray.get(index).getAsJsonObject();
    course_info.setCourseId(jsonOBJ_courseArray.get("courseId").getAsString());
    course_info.setCourseCredit(jsonOBJ_courseArray.get("courseCredit").getAsInt());
    course_info.setTotalCapacity(jsonOBJ_courseArray.get("totalCapacity").getAsInt());
    course_info.setCourseHour(jsonOBJ_courseArray.get("courseHour").getAsInt());
    course_info.setCourseDept(jsonOBJ_courseArray.get("courseDept").getAsString());
    course_info.setCourseName(jsonOBJ_courseArray.get("courseName").getAsString());
  }

  private static void jsonToClass(JsonArray jsonArray, PreparedStatement ps_get_CourseId, int index,
      class_info class_info) throws SQLException {
    //通过json找出需要查询的string:::course_name
    JsonObject jsonOBJ_classArray = jsonArray.get(index).getAsJsonObject();
    String course_Name = jsonOBJ_classArray.get("courseName").getAsString();

    //由于标间外键的存在，插入之前先要查询对应course_Name的courseId
    ps_get_CourseId.setString(1, course_Name);
    ResultSet resultSet = ps_get_CourseId.executeQuery();
    resultSet.next();
    int courseId = resultSet.getInt("id");

    //将class_info对象中的信息插入数据库
    class_info.setCourseId(courseId);
    class_info.setClassName(jsonOBJ_classArray.get("className").getAsString());
  }

  private static void jsonToTeacher(JsonArray jsonArray, PreparedStatement ps_get_classId,
      int index, PreparedStatement ps_addTeacher)
      throws SQLException {
    //通过json找出需要查询的string:::course_name
    JsonObject jsonOBJ_classArray = jsonArray.get(index).getAsJsonObject();
    String class_Name = jsonOBJ_classArray.get("className").getAsString();

    //由于标间外键的存在，插入之前先要查询对应course_Name的courseId
    ps_get_classId.setString(1, class_Name);
    ResultSet resultSet = ps_get_classId.executeQuery();
    resultSet.next();
    int classId = resultSet.getInt("id");

    //将teacher对象中的信息插入数据库
    if (!jsonOBJ_classArray.get("teacher").equals(JsonNull.INSTANCE)) {
      String name = jsonOBJ_classArray.get("teacher").getAsString();
      String temp_name = name.replace(',', ' ');
      String[] name_arr = temp_name.split("\\s+");
      for (String s : name_arr) {
        teacher teacher = new teacher();
        teacher.setName(s);
        teacher.setClassId(classId);
        teacherToDatabase(ps_addTeacher, teacher);
      }
    }
  }

  private static void jsonToPre(JsonArray jsonArray, PreparedStatement ps_get_pre, int index,
      prerequisite prerequisite) throws SQLException {
    JsonObject jsonOBJ_Pre = jsonArray.get(index).getAsJsonObject();
    String course_Name = jsonOBJ_Pre.get("courseName").getAsString();

    // 由于标间外键的存在，插入之前先要查询对应course_Name的courseId
    ps_get_pre.setString(1, course_Name);
    ResultSet resultSet = ps_get_pre.executeQuery();
    if (resultSet.next()) {
      int courseId = resultSet.getInt("id");
      prerequisite.setCourseId(courseId);
    }

    // 检测先修课是否为空并且获取原始prerequisite字符串
    String pre;
    if (jsonOBJ_Pre.get("prerequisite").equals(JsonNull.INSTANCE)) {
      prerequisite.setPrerequisite_list(null);
      return;
    } else {
      pre = jsonOBJ_Pre.get("prerequisite").getAsString();
    }

    // 开始对字符产的转换处理模块
    // 以空格分割
    String[] arr = pre.split("\\s+");

    // 给括号加了间距的新数组
    int ind = 0;   //tem数组每个时刻的位置
    String[] tem = new String[2 * arr.length];
    for (String value : arr) {
      if (value.charAt(0) == '(') {
        tem[ind] = "(";
        ind++;
        tem[ind] = value.substring(1);
      } else if (value.charAt(value.length() - 1) == ')') {
        int length = value.length();
        tem[ind] = value.substring(0, length - 1);
        ind++;
        tem[ind] = ")";
      } else {
        tem[ind] = value;
      }
      ind++;
    }

    // 新字符串
    StringBuilder final_str = new StringBuilder();
    for (int i = 0; i < tem.length; i++) {
      if (tem[i] != null) {
        final_str.append(tem[i]);
        if (i != tem.length - 1) {
          final_str.append(" ");
        }
      } else {
        break;
      }
    }

    // 以空格分割
    String[] new_arr = final_str.toString().split("\\s+");

    // 临时变量
    String temp;

    //替换加入最终数组中
    int[] fin_arr = new int[new_arr.length];
    int t = 0;
    for (int i = 0; i < fin_arr.length; i++) {
      switch (new_arr[i]) {
        case "(":
          fin_arr[t] = -3;
          break;
        case ")":
          fin_arr[t] = -4;
          break;
        case "或者":
          fin_arr[t] = -1; // 或者 -> -1
          break;
        case "并且":
          fin_arr[t] = -2; // 并且 -> -2
          break;
        default: {
//                        if (new_arr[i].equals("化学原理")){
//                            if (i + 1 < new_arr.length && (new_arr[i + 1].equals("A") || new_arr[i + 1].equals("B"))){
//                                fin_arr[t] = "化学原理 " + new_arr[i + 1];
//                                i++;
//                                ps_get_pre.setString(1, fin_arr[t]);
//                            }else {
//                                ps_get_pre.setString(1, new_arr[i]);
//                            }
//                        }
//                        ResultSet result_Set = ps_get_pre.executeQuery();
//                        resultSet.next();
//                        int course_Id = resultSet.getInt("id");
//                        fin_arr[t] = course_Id + "";
          if (i + 1 < new_arr.length && new_arr[i + 1].length() == 1
              && new_arr[i + 1].charAt(0) <= 90
              && new_arr[i + 1].charAt(0) >= 65) {
            temp = new_arr[i] + " " + new_arr[i + 1];
            i++;
            ps_get_pre.setString(1, temp);
          } else {
            ps_get_pre.setString(1, new_arr[i]);
          }
          ResultSet result_Set = ps_get_pre.executeQuery();
          if (result_Set.next()) {
            int course_Id = result_Set.getInt("id");
            fin_arr[t] = course_Id;
          } else {
            t--;
            //这里出现数组越界
            if (t >= 1 && (fin_arr[t - 1] == -1 || fin_arr[t - 1] == -2)) {
              t--;
            }
          }
          break;
        }
      }
      t++;
    }

    // 去重
    boolean[] dup = new boolean[100000];
    for (int i = 0; i < t; i++) {
      if (fin_arr[i] >= 0) {
        if (!dup[fin_arr[i]]) {
          dup[fin_arr[i]] = true;
        } else {
          fin_arr[i] = -5;
          if (fin_arr[i - 1] == -1 || fin_arr[i - 1] == -2) {
            fin_arr[i - 1] = -5;
          }
        }
      }
    }

    // 转化为后序表达式，转化之后的数组长度保存在变量ptr中
    int[] stack = new int[100];
    int top = 0, temp_int, ptr = 0;
    for (int i = 0; i < t; i++) {
      switch (fin_arr[i]) {
        case -1:
        case -2:
        case -3:
          stack[top++] = fin_arr[i];
          break;
        case -4:
          while (true) {
            //数组越界
            if (top >= 1) {
              temp_int = stack[--top];
            } else {
              break;
            }
            if (temp_int == -3) {
              break;
            } else {
              fin_arr[ptr++] = temp_int;
            }
          }
          break;
        case -5:
          break;
        default:
          fin_arr[ptr++] = fin_arr[i];
          break;
      }
    }

    // 数组转化为字符串，待储存
    final_str = new StringBuilder();
    for (int i = 0; i < ptr; i++) {
      final_str.append(fin_arr[i]);
      if (i != ptr - 1) {
        final_str.append(" ");
      }
    }

    //将最终处理完成的字符串放入目标对象中
    prerequisite.setPrerequisite_list(final_str.toString());
  }

  //将java对象中存储的数据导入数据库的组件
  private static void courseToDatabase(PreparedStatement ps_addCourse, course_info course_info)
      throws SQLException {
    ps_addCourse.setInt(1, course_info.getTotalCapacity());
    ps_addCourse.setString(2, course_info.getCourseId());
    ps_addCourse.setInt(3, course_info.getCourseHour());
    ps_addCourse.setInt(4, course_info.getCourseCredit());
    ps_addCourse.setString(5, course_info.getCourseName());
    ps_addCourse.setString(6, course_info.getCourseDept());
    ps_addCourse.executeUpdate();
  }

  private static void classToDatabase(PreparedStatement ps_addClass, class_info class_info)
      throws SQLException {
    ps_addClass.setInt(1, class_info.getCourseId());
    ps_addClass.setString(2, class_info.getClassName());
    ps_addClass.executeUpdate();
  }

  private static void teacherToDatabase(PreparedStatement ps_addTeacher, teacher teacher)
      throws SQLException {
    ps_addTeacher.setInt(1, teacher.getClassId());
    ps_addTeacher.setString(2, teacher.getName());
    ps_addTeacher.executeUpdate();
  }

  public static void preToDatabase(PreparedStatement ps_addPre, prerequisite prerequisite)
      throws SQLException {
    ps_addPre.setInt(1, prerequisite.getCourseId());
    ps_addPre.setString(2, prerequisite.getPrerequisite_list());
    ps_addPre.executeUpdate();
  }

  public static void studentToDatabase(PreparedStatement ps_addTeacher, student student)
      throws SQLException {
    ps_addTeacher.setString(1, student.getName());
    ps_addTeacher.setString(2, student.getSex());
    ps_addTeacher.setString(3, student.getDepartment());
    ps_addTeacher.setInt(4, student.getStudentId());
    ps_addTeacher.executeUpdate();
  }
}