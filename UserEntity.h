#ifndef USER_ENTITY_H
#define USER_ENTITY_H

class UserEntity {
  // Header parameters
  private:
    String login;
    String password;

  public:
    UserEntity(String login, String password) : login(login), password(password){
    }

  String getLogin() {
    return login;
  }

  String getPassword() {
    return password;
  }
};

#endif // USER_ENTITY_H