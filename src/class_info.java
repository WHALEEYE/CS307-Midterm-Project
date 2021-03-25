public class class_info {

  private int id;
  private int courseId;
  private String className;
  private String teacher;

  public class_info() {
  }

  public class_info(int id, int courseId, String className, String teacher) {
    this.id = id;
    this.courseId = courseId;
    this.className = className;
    this.teacher = teacher;
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getCourseId() {
    return courseId;
  }

  public void setCourseId(int courseId) {
    this.courseId = courseId;
  }

  public String getClassName() {
    return this.className;
  }

  public void setClassName(String className) {
    this.className = className;
  }

  public String getTeacher() {
    return this.teacher;
  }

  public void setTeacher(String teacher) {
    this.teacher = teacher;
  }
}
