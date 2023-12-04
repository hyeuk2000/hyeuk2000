#include "command.h"
#include <fstream>
#include <regex>
#include <vector>


#define MAXTXT 16384
std::string cmds[20] = { "ls", "man", "grep", "sort", "awk", 
"bc", "head", "tail", "cat", "cp", 
"mv", "rm", "pwd", "cd", "exit" , 
"gcc", "g++", "make", "mkdir", "rmdir" };

int command::get_type(){
    for(int i=0;i<20;i++){
        if(!strcmp(argv[0],cmds[i].c_str())){
            if(i<6) return 1;
            else if(i<9) return 2;
            else if(i<13) return 3;
            else if(i<15) return 4;
            else return 5;
        }
    }
    return 0;
}

void cmd_type1::execute(){
    if(execvp(argv[0],argv)<0){
        fprintf(stderr,"Shell : Command not found.\n");
		exit(1);
    }
    exit(0);
    return;
}


void cmd_type2::execute(){
    if(strcmp(argv[0],cmds[6].c_str())==0){ //head
        char buf[1024];
        int lines=10;
        int read_len,fd;
        int i, flag=0;

        if(argc!=1 && argc!=2 && argc!=3 && argc!=4){
            printf("Usage : %s [-n K(lines)] [file_path]\n",argv[0]);
            exit(-1);
        }
        if(argc==1) fd=dup(0);
        else if(argc==2) fd=open(argv[1],O_RDONLY);
        else if(argc==3 && !strcmp(argv[1],"-n")){
            lines=atoi(argv[2]);
            fd=dup(0);
        }
        else if(argc==4 && !strcmp(argv[1],"-n")){
            lines=atoi(argv[2]);
            fd=open(argv[3],O_RDONLY);
        }
        else{
            printf("Usage : %s [-n K(lines)] [file_path]\n",argv[0]);
            exit(-1);
        }
        if(fd==-1){
            printf("Shell : No Such File.\n");
            exit(0);
        }
    
        while((read_len=read(fd,buf,1000))>0){
            for(i=0;i<read_len;i++){
                write(1,&buf[i],1);
                if(buf[i]=='\n') lines--;
                if(!lines){ flag=1; break; }
            }
            if(flag) break;
        }
        buf[0]='\n';
        write(1,&buf[0],1);
        close(fd);
        fflush(stdout);
    }
    if(strcmp(argv[0],cmds[7].c_str())==0){ //tail
        char *out=(char*)malloc(sizeof(char)*1024);
        char buf[1024];
        char *std_buf=(char*)malloc(sizeof(char)*1024);
        int lines=10;
        int read_len;
        int fd, size;
        int i, k=1, l=1;

        if(argc!=1 && argc!=2 && argc!=3 && argc!=4){
            printf("Usage : %s [-n K(lines)] [file_path]\n",argv[0]);
            exit(-1);
        }
        if(argc==1) fd=dup(0);
        else if(argc==2) fd=open(argv[1],O_RDONLY);
        else if(argc==3 && !strcmp(argv[1],"-n")){
            lines=atoi(argv[2]);
            fd=dup(0);
        }
        else if(argc==4 && !strcmp(argv[1],"-n")){
            lines=atoi(argv[2]);
            fd=open(argv[3],O_RDONLY);
        }
        else{
            printf("Usage : %s [-n K(lines)] [file_path]\n",argv[0]);
            exit(-1);
        }
        if(fd==-1){
            printf("Shell : No Such File.\n");
            exit(0);
        }
        if(argc==2 || argc==4){
            size=(int)lseek(fd,0,SEEK_END);
            lseek(fd,0,SEEK_END);
            lseek(fd,-1,SEEK_CUR);
        }
        else{
            for(i=0;read(fd,std_buf+i,1);i++){
                if(i>=1000*l) std_buf=(char *)realloc(std_buf,sizeof(char)*1024*(++l));
            }
            size=strlen(std_buf);
        }
        for(i=0;i<size;i++){
            if(argc==2 || argc==4) read_len=read(fd,buf,1);
            else buf[0]=std_buf[strlen(std_buf)-1-i];
            if(buf[0]=='\n') lines--;
            if(!lines) break;
            if(i+10>1024*k) out=(char*)realloc(out,sizeof(char)*1024*(++k));
            out[i]=buf[0];
            if(argc==2 || argc==4) lseek(fd,-2,SEEK_CUR);
        }
        int m;
        char *temp = (char*)malloc(sizeof(char)*strlen(out)+10);
        memcpy(temp,out,strlen(out));
        for(m=strlen(out)-1;m>=0;m--){
            out[strlen(out)-1-m]=temp[m];
        }
        free(temp);
        write(1,out,strlen(out));
        free(out);
        if(argc!=2 && argc!=4) free(std_buf);
        buf[0]='\n';
        write(1,&buf[0],1);
        fflush(stdout);    
    }
    if(strcmp(argv[0],cmds[8].c_str())==0){ //cat
        char buf[1024];
        int read_len;
        int fd;
        if(argc!=1 && argc!=2){
            printf("Usage : %s [file_path]\n",argv[0]);
            exit(-1);
        }
        if(argc==2) fd=open(argv[1],O_RDONLY);
        else fd=dup(0);
        if(fd==-1){
            printf("Shell : No Such File.\n");
            exit(0);
        }
        while((read_len=read(fd,buf,1000))>0) write(1,buf,read_len);
        close(fd);
        buf[0]='\n';
        write(1,&buf[0],1);
        fflush(stdout);
    }
    exit(0);
    return;
}

void cmd_type3::execute(){
    if(strcmp(argv[0],cmds[9].c_str())==0){ //cp
        char buf[1024];
        int read_len;
        int fd_src, fd_dst;
        if(argc!=3){
            printf("Usage : %s src dst\n",argv[0]);
            exit(-1);
        }
        fd_src=open(argv[1],O_RDONLY);
        fd_dst=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH);
        
        while((read_len=read(fd_src,buf,1000))>0) write(fd_dst,buf,read_len);
        close(fd_src);
        close(fd_dst);
    }
    if(strcmp(argv[0],cmds[10].c_str())==0){ //mv
        if(argc!=3) printf("Usage : %s old_name new_name\n",argv[0]);
        else if(rename(argv[1],argv[2]) != 0){
            if(errno==EACCES){
                fprintf(stderr,"%s: Permission denied\n",argv[0]);
            }
            else if(errno==EISDIR){
                fprintf(stderr,"%s: Is a directory\n",argv[0]);
            }
            else if(errno==ENOENT){
                fprintf(stderr,"%s: No such file or directory\n",argv[0]);
            }
            else if(errno==ENOTDIR){
                fprintf(stderr,"%s: Not a directory\n",argv[0]);
            }
            else if(errno==EPERM){
                fprintf(stderr,"%s: Permission denied\n",argv[0]);
            }
            else{
                fprintf(stderr,"%s: Error occurred: <%d>\n",argv[0],errno);
            }
            exit(-1);
        }
    }
    if(strcmp(argv[0],cmds[11].c_str())==0){ //rm
        if(argc!=2) printf("Usage : %s path\n",argv[0]);
        else if(unlink(argv[1]) != 0){
            if(errno==EACCES){
                fprintf(stderr,"%s: Permission denied\n",argv[0]);
            }
            else if(errno==EISDIR){
                fprintf(stderr,"%s: Is a directory\n",argv[0]);
            }
            else if(errno==ENOENT){
                fprintf(stderr,"%s: No such file or directory\n",argv[0]);
            }
            else if(errno==ENOTDIR){
                fprintf(stderr,"%s: Not a directory\n",argv[0]);
            }
            else if(errno==EPERM){
                fprintf(stderr,"%s: Permission denied\n",argv[0]);
            }
            else{
                fprintf(stderr,"%s: Error occurred: <%d>\n",argv[0],errno);
            }
            exit(-1);
        }
    }
    if(strcmp(argv[0],cmds[12].c_str())==0){ //pwd
        char buf[MAXSTR];
        if(argc!=1) printf("Usage : %s\n",argv[0]);
        else if(getcwd(buf,MAXSTR) == NULL){
            fprintf(stderr, "current working directory get error: %d\n", errno);
            exit(-1);
        }
        write(1,buf,strlen(buf));
        write(1,"\n",1);
    }
    exit(0);
    return;
}

void cmd_type4::execute(){
    if(strcmp(argv[0],cmds[13].c_str())==0){ //cd
        if(!argv[1]){
			if(chdir(getenv("HOME")) != 0){
				if(errno==EACCES){
					fprintf(stderr,"%s: Permission denied\n",argv[0]);
				}
				else if(errno==EISDIR){
					fprintf(stderr,"%s: Is a directory\n",argv[0]);
				}
				else if(errno==ENOENT){
					fprintf(stderr,"%s: No such file or directory\n",argv[0]);
				}
				else if(errno==ENOTDIR){
					fprintf(stderr,"%s: Not a directory\n",argv[0]);
				}
				else if(errno==EPERM){
					fprintf(stderr,"%s: Permission denied\n",argv[0]);
				}
				else{
					fprintf(stderr,"%s: Error occurred: <%d>\n",argv[0],errno);
				}
			}
		}
		else{
			if(chdir(argv[1]) != 0){
				if(errno==EACCES){
					fprintf(stderr,"%s: Permission denied\n",argv[0]);
				}
				else if(errno==EISDIR){
					fprintf(stderr,"%s: Is a directory\n",argv[0]);
				}
				else if(errno==ENOENT){
					fprintf(stderr,"%s: No such file or directory\n",argv[0]);
				}
				else if(errno==ENOTDIR){
					fprintf(stderr,"%s: Permission denied\n",argv[0]);
				}
				else if(errno==EPERM){
					fprintf(stderr,"%s: Permission denied\n",argv[0]);
				}
				else{
					fprintf(stderr,"%s: Error occurred: <%d>\n",argv[0],errno);
				}
			}
		}
    }
    if(strcmp(argv[0],cmds[14].c_str())==0){ //exit
        int ret=0;
		if(argv[1]) ret=atoi(argv[1]);
		fprintf(stderr,"exit\n");
		exit(ret);
    }
    return;
}

std::vector<std::string> get_fileContents(const std::vector<std::string>& inputFiles){
    using namespace std;

    vector<string> fileContents;    // 파일 전체를 저장할 문자열 배열
    string fileContent;             // 파일 내용을 저장할 문자열

    for(auto& inputFile : inputFiles){
        fileContent = "";
        ifstream readFile(inputFile);   // 파일을 읽기 모드로 열기

        // 파일이 정상적으로 열렸는지 확인
        if (!readFile.is_open()) {
            cerr << "파일을 열 수 없습니다.\n";
            return fileContents;
        }

        // 파일 내용을 읽어와 문자열에 저장
        char ch;
        while (readFile.get(ch)) {
            fileContent += ch;
        }

        fileContents.push_back(fileContent);
        
        // 파일 닫기
        readFile.close();
    }

    // 파일 내용을 반환
    return fileContents;
}

std::vector<std::string> findMatchingStrings(const std::vector<std::string>& inputStrings, const std::string& regexPattern)
{
    using namespace std;

    vector<string> matchingStrings;

    //정규표현식 패턴으로 객체 생성
    regex pattern(regexPattern);

    for(const auto& inputString: inputStrings){
        // 현재 문자열이 정규 표현식과 매치되는지 확인
        if(regex_search(inputString, pattern)){
            matchingStrings.push_back(inputString);
        }
    }

    return matchingStrings;
}

void reviseCString(int type, std::vector<std::string>& stringArray) {
    std::regex pattern;
    if(type==1) pattern = std::regex("\\.c$"); // gcc에서 입력되는 경우
    else        pattern = std::regex("\\.c(|c|pp)$"); // 정규표현식: 문자열 끝이 ".c .cc .cpp"인 경우

    for(auto& str: stringArray){
        // 현재 문자열이 정규 표현식과 매치되는지 확인
        if(regex_search(str, pattern)){
            str = std::string("revise_")+str;
        }
    }
}

void executeShellScript(const std::vector<std::string>& command) {
    std::string fullCommand;

    // 벡터의 각 문자열을 공백으로 구분된 하나의 문자열로 결합
    for (const auto& arg : command) {
        fullCommand += arg + " ";
    }

    int result = std::system(fullCommand.c_str());

    if (result == -1) {
        // 실행에 실패한 경우
        std::cerr << "쉘 스크립트 실행에 실패했습니다." << std::endl;
    }
}


void cmd_type5::execute(){
    using namespace std;

    vector<string> fileContents;
    vector<string> CFiles;
    vector<string> inputCommands;
    string temp = "You are software engineer. Get this code and revise it. Make sure that you're answer must contain nothing but the revised code.\n";
    if(!strcmp(argv[0],cmds[15].c_str())){ //gcc
        int pid;
        int status;
        //gcc를 한 번 실행시켜 봄.
        if((pid=fork())==0){
            execvp(argv[0],argv);
        }
        else{
            waitpid(pid,&status,0);
            //만약 에러가 검출되어 디버깅이 필요하다면
            if(status!=0){
                //조건에 맞는 파일 (.c파일)의 이름과 코드를 저장.
                for(int i=0;i<argc;i++) inputCommands.push_back(string(argv[i]));
                CFiles = findMatchingStrings(inputCommands, string(R"(\.c$)"));
                fileContents = get_fileContents(CFiles);

                // // openai api로 코드 전송
                for(auto& c : fileContents){
                    temp = temp + c;
                    query q(temp);
                    c = q.get_output();
                
                }
                // // openai api로 수정된 코드 받기.

                // 별도의 파일로 출력
                for(long unsigned int i=0;i<CFiles.size();i++){
                    // 파일을 쓰기 모드로 열기, 파일이 없으면 생성
                    string outputFilePath = string("revise_")+CFiles[i];
                    ofstream outputFile(outputFilePath, ios::out);

                    // 출력할 문자열
                    string outputString = fileContents[i];

                    // 파일에 문자열 쓰기
                    outputFile << outputString;

                    // 파일 닫기
                    outputFile.close();
                }
                cout<<endl;
                //기존 a.c 파일을 revise_a.c파일로 변경.
                reviseCString(1, inputCommands);

                //수정된 파일로 다시 gcc 실행.
                executeShellScript(inputCommands);
            }
        }
    }
    if(!strcmp(argv[0],cmds[16].c_str())){ //g++
        int pid;
        int status;
        if((pid=fork())==0){
            execvp(argv[0],argv);
        }
        else{
            waitpid(pid,&status,0);
            if(status!=0){
                for(int i=0;i<argc;i++) inputCommands.push_back(string(argv[i]));
                CFiles = findMatchingStrings(inputCommands, string(R"(\.c(|c|pp)$)"));
                fileContents = get_fileContents(CFiles);

                // // openai api로 전송
                for(auto& c : fileContents){
                    temp = temp + c;
                    query q(temp);
                    c = q.get_output();
                
                }
                // // openai api로 받기
                // // 코드 부분만 파싱

                // 별도의 파일로 출력
                for(long unsigned int i=0;i<CFiles.size();i++){
                    // 파일을 쓰기 모드로 열기, 파일이 없으면 생성
                    string outputFilePath = string("revise_")+CFiles[i];
                    ofstream outputFile(outputFilePath, ios::out);

                    // 출력할 문자열
                    string outputString = fileContents[i];

                    // 파일에 문자열 쓰기
                    outputFile << outputString;

                    // 파일 닫기
                    outputFile.close();
                }
                cout<<endl;
                reviseCString(2, inputCommands);

                executeShellScript(inputCommands);
            }
        }
    }
    if(strcmp(argv[0],cmds[17].c_str())==0){ //make
        
    }
    if(strcmp(argv[0],cmds[18].c_str())==0){ //mkdir
        if(argc!=2) printf("Usage : %s path\n",argv[0]);
        else{
            if(mkdir(argv[1],0755) != 0){
                if(errno==EACCES){
                    fprintf(stderr,"%s: Permission denied\n",argv[0]);
                }
                else if(errno==EISDIR){
                    fprintf(stderr,"%s: Is a directory\n",argv[0]);
                }
                else if(errno==ENOENT){
                    fprintf(stderr,"%s: No such file or directory\n",argv[0]);
                }
                else if(errno==ENOTDIR){
                    fprintf(stderr,"%s: Not a directory\n",argv[0]);
                }
                else if(errno==EPERM){
                    fprintf(stderr,"%s: Permission denied\n",argv[0]);
                }
                else{
                    fprintf(stderr,"%s: Error occurred: <%d>\n",argv[0],errno);
                }
                exit(-1);
            } 
        }
    }
    if(strcmp(argv[0],cmds[19].c_str())==0){ //rmdir
        if(argc!=2) printf("Usage : %s path\n",argv[0]);
        else{
            if(rmdir(argv[1]) != 0){
                if(errno==EACCES){
                    fprintf(stderr,"%s: Permission denied\n",argv[0]);
                }
                else if(errno==EISDIR){
                    fprintf(stderr,"%s: Is a directory\n",argv[0]);
                }
                else if(errno==ENOENT){
                    fprintf(stderr,"%s: No such file or directory\n",argv[0]);
                }
                else if(errno==ENOTDIR){
                    fprintf(stderr,"%s: Not a directory\n",argv[0]);
                }
                else if(errno==EPERM){
                    fprintf(stderr,"%s: Permission denied\n",argv[0]);
                }
                else{
                    fprintf(stderr,"%s: Error occurred: <%d>\n",argv[0],errno);
                }
                exit(-1);
            } 
        }
    }
    
    exit(0);
    return;
    
}
