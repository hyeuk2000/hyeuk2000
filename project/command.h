#include <iostream>
#include <string>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "../include/openai/openai.hpp"
#define MAXARGS 256
#define MAXSTR 512
#define MAXTOK 4096
class query{

    public:
        template <typename T>
        //openai로 수정된 코드 받아오기
        query(const T& input){
            openai::start();
            openai::_detail::Json str = {
                {"model", "gpt-4"},
                {"messages", {{{"role","user"},{"content",input}}}},
                {"max_tokens", MAXTOK},
                {"temperature", 0}
            };
            auto chat = openai::chat().create(str);
            output = chat.dump(2);
        }
        //수정된 코드 parsing해서 정상 내용으로 만든 후 return
        std::string get_output(){
            openai::_detail::Json data = openai::_detail::Json::parse(output);
            q_output = data["choices"][0]["message"]["content"].dump();
            q_output.erase(q_output.begin());
            q_output.erase(q_output.end()-1);
            int check=1;
            int count=0;
            for(auto& i : q_output){
                if(i==92){
                    if(check==1){
                        if(q_output[count+1]==110){
                            q_output.replace(count,2,"\n");
                        }
                        else if(q_output[count+1]==116){
                            q_output.replace(count,2,"\t");
                        }   
                        else{
                            if((q_output[count+1]==39)||(q_output[count+1]==34)){
                                check *= -1;
                            }
                            q_output.erase(count,1);

                        }
                    }
                    else if(check==-1){
                        if((q_output[count+1]==39)||(q_output[count+1]==34)){
                                check *= -1;
                                q_output.erase(count,1);
                        }
                        else if(q_output[count+1]==92){
                            q_output.erase(count,1);
                        }
                    }
                }
                count++;
            }
            return q_output;
        }
    private:
        std::string output;
        std::string q_output;
};

class command{
    public:
        command(){
            argc=0;
            input = NULL;
            output = NULL;
            app_output = NULL;
            for(int i=0;i<MAXARGS;i++){
                argv[i] = NULL;
            }
        }
        
        //copy constructor 필요!
        template <typename T>
        command(const T& input_command){
            *this = input_command;
        }

        //operator override (=) 필요!
        template <typename T>
        command operator =(const T& input_command){
            command* temp = new command;

            temp->argc = input_command.argc;
            temp->input = input_command.input;
            temp->output = input_command.output;
            temp->app_output = input_command.app_output;
            for(int i=0;i<MAXARGS;i++){
                temp->argv[i]=input_command.argv[i];
            }

            return *temp;
        }

        // Setter & Getter
        virtual void execute(){
            if (execv(argv[0], argv) < 0) {
                fprintf(stderr,"Shell: Command not found.\n");
                exit(0);
            }
            exit(0);
            return;
        }
        int get_type();        
        void set_argc(int _argc){ argc = _argc; }
        void set_input(char * arg){ input = arg; }
        void set_output(char * arg){ output = arg; }
        void set_appoutput(char * arg){ app_output = arg; }
        void set_argv(char * arg){ argv[argc++]=arg; }
        int get_argc() const { return argc; }
        char* get_input() const { return input; }
        char* get_output() const { return output; }
        char* get_app_output() const { return app_output; }
        char* get_argv_element(int index) const { return argv[index]; }
        
    protected:
        int argc;

        // background, pipe, input, output, app_output -> 각각 어떤 역할을 하는 멤버인지 간단한 설명 부탁!
        char *input;
        char *output;
        char *app_output;
        // background, pipe, input, output, app_output
        
        //arguments
        char * argv[MAXARGS];
        //arguments
};

class cmd_type1 : public command{
    public:
        cmd_type1(const command& _command){
            argc = _command.get_argc();
            input = _command.get_input();
            output = _command.get_output();
            app_output = _command.get_app_output();
            for(int i=0;i<MAXARGS;i++){
                argv[i] = _command.get_argv_element(i);
            }
        }
        void execute();
};
class cmd_type2 : public command{
    public:
        cmd_type2(const command& _command){
            argc = _command.get_argc();
            input = _command.get_input();
            output = _command.get_output();
            app_output = _command.get_app_output();
            for(int i=0;i<MAXARGS;i++){
                argv[i] = _command.get_argv_element(i);
            }
        }

        void execute();
};
class cmd_type3 : public command{
    public:
        cmd_type3(const command& _command){
            argc = _command.get_argc();
            input = _command.get_input();
            output = _command.get_output();
            app_output = _command.get_app_output();
            for(int i=0;i<MAXARGS;i++){
                argv[i] = _command.get_argv_element(i);
            }
        }

        void execute();
};
class cmd_type4 : public command{
    public:
        cmd_type4(const command& _command){
            argc = _command.get_argc();
            input = _command.get_input();
            output = _command.get_output();
            app_output = _command.get_app_output();
            for(int i=0;i<MAXARGS;i++){
                argv[i] = _command.get_argv_element(i);
            }
        }

        void execute();
};
class cmd_type5 : public command{ //새로 만든 커맨드. (gcc, g++, make등)
    public:
        cmd_type5(const command& _command){
            argc = _command.get_argc();
            input = _command.get_input();
            output = _command.get_output();
            app_output = _command.get_app_output();
            for(int i=0;i<MAXARGS;i++){
                argv[i] = _command.get_argv_element(i);
            }
        }

        void execute();
};
class cmd_type0 : public command{ //정의되지 않은 커맨드이거나, ./main.out 등의 파일 실행 커맨드
    public:
        cmd_type0(const command& _command){
            argc = _command.get_argc();
            input = _command.get_input();
            output = _command.get_output();
            app_output = _command.get_app_output();
            for(int i=0;i<MAXARGS;i++){
                argv[i] = _command.get_argv_element(i);
            }
        }
};
