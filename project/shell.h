#include "command.h"

class shell{
    public:
        shell(){
            argc=0;
            cmd_count=0;
            bg=0;
            if(getcwd(cur_loc,MAXSTR) == NULL){
                fprintf(stderr, "Starting error: %d\n", errno);
            }
            std::cout << cur_loc << "> ";

            char * buf = NULL;
            size_t size;
            getline(&buf,&size,stdin);
            buf[strlen(buf)-1]='\0';
            strcpy(str,buf);
        }
        ~shell(){
            //free
            for(int i=0;i<cmd_count;i++){
                delete [] pipe_fd[i];
            }
            delete [] pid;
            delete [] cmd;
            //free
        }
        void execute(){
            //argument parse
            char * ptr = strtok(str, " ");
            while(ptr!=NULL){
                argv[argc++]=ptr;
                ptr = strtok(NULL, " ");
            }
            argv[argc]=NULL;
            //argument parse
            //check number of command
            if(argc==0) return;
            for(int i=0;i<argc;i++){
                if(!strcmp(argv[i],"|")) cmd_count++;
            }

            cmd = new command[cmd_count+1];
            pid = new pid_t[cmd_count+1];
            if(cmd_count){
                pipe_fd = new int*[cmd_count];
                for(int i=0;i<cmd_count;i++) pipe_fd[i] = new int[2];
            }
            for(int i=0;i<cmd_count;i++){
                if(pipe(pipe_fd[i])<0){
                    perror("pipe error");
			        exit(-1);
                }
            }
            //check number of command
            
            //parsing input, output, appending output, background and pipe line
            cmd_count=0;
            for(int i=0;i<argc;i++){
                if(!strcmp(argv[i],"|")){
                    cmd_count++;
                }
                else if(!strcmp(argv[i],"<")){
                    cmd[cmd_count].set_input(argv[++i]);
                }
                else if(!strcmp(argv[i],">")){
                    cmd[cmd_count].set_output(argv[++i]);
                }
                else if(!strcmp(argv[i],">>")){
                    cmd[cmd_count].set_appoutput(argv[++i]);
                }
                else if(!strcmp(argv[i],"&")){
                    bg=1;
                }
                else {
                    cmd[cmd_count].set_argv(argv[i]);
                }
            }
            //parsing input, output, appending output, background and pipe line
            //execute
            for(int i=0;i<=cmd_count;i++){
                if(cmd[i].get_type()==1){
                    if((pid[i]=fork())==0){
                        int fd, fd2;
                        if(cmd_count){
                            for(int j=0;j<cmd_count;j++){
						        if(j!=i) close(pipe_fd[j][1]);
						        if(i>0 && j!=i-1) close(pipe_fd[j][0]);
						        else if(i==0) close(pipe_fd[j][0]);
					        }
					        if(i!=cmd_count) dup2(pipe_fd[i][1],1);
					        if(i) dup2(pipe_fd[i-1][0],0);
                        }
                        if(!cmd[i].get_input()){
					        fd=open(cmd[i].get_input(),O_RDONLY); 
					        dup2(fd,0);
				        }
				        if(!cmd[i].get_output()){
					        fd2=open(cmd[i].get_output(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
				        else if(!cmd[i].get_app_output()){
					        fd2=open(cmd[i].get_app_output(),O_RDWR|O_CREAT|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
                        cmd_type1 cmd1(cmd[i]);
                        cmd1.execute();
                    }
                    continue;
                }
                else if(cmd[i].get_type()==2){
                    if((pid[i]=fork())==0){
                        int fd, fd2;
                        if(cmd_count){
                            for(int j=0;j<cmd_count;j++){
						        if(j!=i) close(pipe_fd[j][1]);
						        if(i>0 && j!=i-1) close(pipe_fd[j][0]);
						        else if(i==0) close(pipe_fd[j][0]);
					        }
					        if(i!=cmd_count) dup2(pipe_fd[i][1],1);
					        if(i) dup2(pipe_fd[i-1][0],0);
                        }
                        if(!cmd[i].get_input()){
					        fd=open(cmd[i].get_input(),O_RDONLY); 
					        dup2(fd,0);
				        }
				        if(!cmd[i].get_output()){
					        fd2=open(cmd[i].get_output(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
				        else if(!cmd[i].get_app_output()){
					        fd2=open(cmd[i].get_app_output(),O_RDWR|O_CREAT|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
                        cmd_type2 cmd2(cmd[i]);
                        cmd2.execute();
                    }
                    continue;
                }
                else if(cmd[i].get_type()==3){
                    if((pid[i]=fork())==0){
                        int fd, fd2;
                        if(cmd_count){
                            for(int j=0;j<cmd_count;j++){
						        if(j!=i) close(pipe_fd[j][1]);
						        if(i>0 && j!=i-1) close(pipe_fd[j][0]);
						        else if(i==0) close(pipe_fd[j][0]);
					        }
					        if(i!=cmd_count) dup2(pipe_fd[i][1],1);
					        if(i) dup2(pipe_fd[i-1][0],0);
                        }
                        if(!cmd[i].get_input()){
					        fd=open(cmd[i].get_input(),O_RDONLY); 
					        dup2(fd,0);
				        }
				        if(!cmd[i].get_output()){
					        fd2=open(cmd[i].get_output(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
				        else if(!cmd[i].get_app_output()){
					        fd2=open(cmd[i].get_app_output(),O_RDWR|O_CREAT|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
                        cmd_type3 cmd3(cmd[i]);
                        cmd3.execute();
                    }
                    continue;
                }
                else if(cmd[i].get_type()==4){
                    cmd_type4 cmd4(cmd[i]);
                    cmd4.execute();
                    continue;
                }
                else if(cmd[i].get_type()==5){
                    if((pid[i]=fork())==0){
                        int fd, fd2;
                        if(cmd_count){
                            for(int j=0;j<cmd_count;j++){
						        if(j!=i) close(pipe_fd[j][1]);
						        if(i>0 && j!=i-1) close(pipe_fd[j][0]);
						        else if(i==0) close(pipe_fd[j][0]);
					        }
					        if(i!=cmd_count) dup2(pipe_fd[i][1],1);
					        if(i) dup2(pipe_fd[i-1][0],0);
                        }
                        if(!cmd[i].get_input()){
					        fd=open(cmd[i].get_input(),O_RDONLY); 
					        dup2(fd,0);
				        }
				        if(!cmd[i].get_output()){
					        fd2=open(cmd[i].get_output(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
				        else if(!cmd[i].get_app_output()){
					        fd2=open(cmd[i].get_app_output(),O_RDWR|O_CREAT|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
                        cmd_type5 cmd5(cmd[i]);
                        cmd5.execute();
                    }
                }
                else if(cmd[i].get_type()==0){
                    if((pid[i]=fork())==0){
                        int fd, fd2;
                        if(cmd_count){
                            for(int j=0;j<cmd_count;j++){
						        if(j!=i) close(pipe_fd[j][1]);
						        if(i>0 && j!=i-1) close(pipe_fd[j][0]);
						        else if(i==0) close(pipe_fd[j][0]);
					        }
					        if(i!=cmd_count) dup2(pipe_fd[i][1],1);
					        if(i) dup2(pipe_fd[i-1][0],0);
                        }
                        if(!cmd[i].get_input()){
					        fd=open(cmd[i].get_input(),O_RDONLY); 
					        dup2(fd,0);
				        }
				        if(!cmd[i].get_output()){
					        fd2=open(cmd[i].get_output(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
				        else if(!cmd[i].get_app_output()){
					        fd2=open(cmd[i].get_app_output(),O_RDWR|O_CREAT|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH); 
					        dup2(fd2,1);
				        }
                        cmd_type0 cmd0(cmd[i]);
                        cmd0.execute();
                    }
                    continue;
                }
                else{
                    fprintf(stderr,"shell: Command not found.\n");
                }
            }
            for(int i=0;i<cmd_count;i++){
		        close(pipe_fd[i][0]);
		        close(pipe_fd[i][1]);
	        } 
            if (!bg) {
		        for(int i=0;i<=cmd_count;i++){
                    if(cmd[i].get_type()==4) continue;
				    int status;
				    if (waitpid(pid[i], &status, 0) < 0)
					    printf("waitfg: waitpid error\n");
			    }
		    }
            //execute
            return;
        }


    private:
        int bg;
        char str[MAXSTR]; // input string
        int argc; // number of arguments
        char *argv[MAXARGS]; // arguments
        int cmd_count; //count of command;
        pid_t *pid; //pid of child process, number is same as cmd_count;
        command * cmd; //commands
        int **pipe_fd;
        char cur_loc[MAXSTR];
};