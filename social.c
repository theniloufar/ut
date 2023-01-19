#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct user user;
typedef struct post post;

void signup(user *head, int loginflag);
void login(user *head, int *login1, int count, int *loginflag, char *loginname);
int scan(user *head, int count, int *login1, int *postcount);
user *create_user(char *new_username, char *new_password, user *head);
int search_user(char *new_username, user *head);
int logout(int loginflag);
int posting(char *username, user *headtemp);
void like(user *head, int numofpost, char *loginname);
int delete (user *headdelete, char *username);
void info(user *head, int loginflag, char *loginname, int count);
void find_user(int count, user *head);
void write_file(user *head);

struct post
{
    char *username;
    int post_id;
    int post_likes;
    char *post_txt;
    struct post *next_post;
    char **people;
};
struct user
{
    char *username;
    char *password;
    struct post *post;
    struct user *next_user;
};

int scan(user *head, int count, int *login1, int *postcount)
{
    char *loginname;
    user *headpost = head;
    user *headdelete = head;
    loginname = (char *)malloc(10 * sizeof(char));
    int loginflag = 0;
    while (1)/*getting the order*/
    {   
        int i = 0;
        int x;
        char a;
        printf("enter your order:\n");
        char *order = (char *)malloc(sizeof(char));
        a = getchar();
        while (a != ' ' && a != '\n')
        {
            order[i] = a;
            i++;
            order = (char *)realloc(order, (i + 1) * sizeof(char));
            a = getchar();
        }
        order = (char *)realloc(order, sizeof(char) * (i + 2));
        order[i] = '\0';
        if (strcmp(order, "signup") == 0)
        {
            signup(head, loginflag);
            free(order);
            fflush(stdin);
            count++;
        }
        else if (strcmp(order, "login") == 0)
        {
            login(head, login1, count, &loginflag, loginname);
            free(order);
            fflush(stdin);
        }
        else if (strcmp(order, "logout") == 0)
        {
            loginflag = logout(loginflag);
            free(order);
            free(loginname);
            loginname = (char *)malloc(sizeof(char));
            fflush(stdin);
        }
        else if (strcmp(order, "post") == 0)
        {
            posting(loginname, headpost);
            (*postcount)++;
            free(order);
            fflush(stdin);
        }
        else if (strcmp(order, "like") == 0)
        {
            like(head, *postcount, loginname);
            free(order);
            fflush(stdin);
        }
        else if (strcmp(order, "find_user") == 0)
        {
            find_user(count, head);
            free(order);
            fflush(stdin);
        }
        else if (strcmp(order, "delete") == 0)
        {
            delete (headdelete, loginname);
            free(order);
            fflush(stdin);
        }
        else if (strcmp(order, "info") == 0)
        {
            info(head, loginflag, loginname, count);
            free(order);
            fflush(stdin);
        }
        write_file(head);
    }
}

void info(user *head, int loginflag, char *loginname, int count)
{
    if (loginflag == 0)
    {
        printf("you should login first\n");
        return;
    }
    user *temp = head;
    temp = temp->next_user;

    int k = 0;
    while (count > k)
    {
        if ((strcmp(temp->username, loginname)) == 0)
        {
            post *temppost = temp->post->next_post;
            printf("username : %s\n", temp->username);/*show profiles username*/
            printf("password : %s\n", temp->password);/*show profiles password*/
            while (temppost != NULL)/*show profiles posts & likes*/
            {
                printf("%d :", temppost->post_id);
                printf("%s\n", temppost->post_txt);
                printf("likes:%d\n", temppost->post_likes);
                temppost = temppost->next_post;
            }
            return;
        }
        temp = temp->next_user;
        k++;
    }
    printf("username not availble\n");
    return;
}

int logout(int loginflag)
{
    if (loginflag == 0)
    {
        printf("you are not logged in\n");
        return 0;
    }
    else
    {
        printf("you logged out\n");
        return 0;
    }
}

void signup(user *head, int loginflag)
{
    if (loginflag == 1)/*loginflag shows if youre logged in*/
    {
        printf("you logged in before\n");
        return;
    }
    int i = 0;
    char *username = (char *)malloc(sizeof(char));
    char *password = (char *)malloc(sizeof(char));
    while ((username[i] = getchar()) != ' ')
    {
        i++;
        username = (char *)realloc(username, (i + 1) * sizeof(char));
    }
    username[i] = '\0';
    int j = 0;
    while ((password[j] = getchar()) != '\n')
    {
        j++;
        password = (char *)realloc(password, (j + 1) * sizeof(char));
    }
    password[j] = '\0';

    int value = search_user(username, head);/*check if the selected username already exists*/
    if (value == 0)
    {
        printf("this username already exists\nplease try again\n");
    }
    else if (value != 0)
    {
        user *user = create_user(username, password, head);/*add a node for new username*/
        user->post = (post *)malloc(sizeof(post));
        user->post->next_post = NULL;
        printf("your account is created\n");
    }
}

user *create_user(char *new_username, char *new_password, user *head)
{
    int i = 0, j = 0;
    if (head->next_user == NULL)
    {
        head->next_user = (user *)malloc(sizeof(user));
        head->next_user->username = (char *)malloc(strlen(new_username) * sizeof(char));/*create the profile usernames node*/
        head->next_user->password = (char *)malloc(strlen(new_password) * sizeof(char));/*create the profile passwords node*/
        while (i != strlen(new_username))
        {
            head->next_user->username[i] = *(new_username + i);
            i++;
        }
        head->next_user->username[i] = *(new_username + i);
        i = 0;
        while (j != strlen(new_password))
        {
            head->next_user->password[j] = *(new_password + j);
            j++;
        }
        head->next_user->password[j] = *(new_password + j);
        j = 0;

        head->next_user->next_user = NULL;

        return head->next_user;
    }
    else if (head->next_user != NULL)
    {
        user *temp = head->next_user;
        while (temp->next_user != NULL)
        {
            temp = temp->next_user;
        }
        temp->next_user = (user *)malloc(sizeof(user));
        temp->next_user->username = new_username;
        temp->next_user->password = new_password;
        temp->next_user->next_user = NULL;
        return temp->next_user;
    }
}

int search_user(char *new_username, user *head)
{
    user *temp = head->next_user;
    while (temp != NULL)
    {
        if ((strcmp(temp->username, new_username)) == 0)
        {
            return 0;
        }
        temp = temp->next_user;
    }
    return 1;
}

void login(user *head, int *login1, int count, int *loginflag, char *loginname)
{
    if (*loginflag == 1)
    {
        printf("you logged in before \n");
        return;
    }
    int k = 0;
    int i = 0;
    char *username = (char *)malloc(sizeof(char));
    char *password = (char *)malloc(sizeof(char));
    while ((username[i] = getchar()) != ' ')
    {
        i++;
        username = (char *)realloc(username, (i + 1) * sizeof(char));
    }
    username[i] = '\0';
    int j = 0;
    while ((password[j] = getchar()) != '\n')
    {
        j++;
        password = (char *)realloc(password, (j + 1) * sizeof(char));
    }
    password[j] = '\0';
    user *temp = head->next_user;
    while (temp)
    {
        if ((strcmp(temp->username, username) == 0) && (strcmp(temp->password, password) == 0))
        {
            printf("you logged in\n");
            *login1 = 1;
            *loginflag = 1;
            int i = 0;
            while (i != strlen(username))
            {
                loginname[i] = *(username + i);
                i++;
            }
            loginname[i] = *(username + i);
            return;
        }
        temp = temp->next_user;
        k++;
    }
    *loginname = username;
    printf("username is invalid\n");
    return;
}

int posting(char *username, user *headpost)
{
    int post_id = 1;
    user *temp = headpost;
    int counterpost = 0;/*posts numbers*/
    char *post_txt = (char *)malloc(sizeof(char));
    post_txt[counterpost] = getchar();
    while (post_txt[counterpost] != '\n')
    {
        counterpost++;
        post_txt = (char *)realloc(post_txt, (counterpost + 1) * sizeof(char));
        post_txt[counterpost] = getchar();
    }
    post_txt[counterpost] = '\0';
    while (strcmp(temp->next_user->username, username) != 0)
    {
        temp = temp->next_user;
    }
    post *temppost = temp->next_user->post;
    while (temppost->next_post != NULL)
    {
        post_id++;
        temppost = temppost->next_post;
    }
    post *new_post = malloc(sizeof(post));
    new_post->post_txt = post_txt;
    new_post->post_likes = 0;
    new_post->post_id = post_id;
    new_post->username = username;
    new_post->next_post = NULL;
    new_post->people = (char **)malloc(sizeof(char *));/*keep people who liked a post in check*/
    new_post->people[0] = NULL;
    temppost->next_post = (post *)malloc(sizeof(post));
    temppost->next_post = new_post;
    return 1;
}

void find_user(int count, user *head)
{
    int i = 0;
    int k = 0;
    char *name = (char *)malloc(1 * sizeof(char));
    name[i] = getchar();
    while (name[i] != '\n')
    {
        i++;
        name = (char *)realloc(name, (i + 1) * sizeof(char));
        name[i] = getchar();
    }
    name[i] = '\0';

    user *temp = head;
    while (count > k)
    {
        if ((strcmp(temp->next_user->username, name) == 0))
        {
            post *temppost = temp->next_user->post->next_post;
            printf("name : %s \n", temp->next_user->username);
            while (temppost != NULL)
            {
                printf("%d :", temppost->post_id);
                printf("%s\n", temppost->post_txt);
                printf("like:%d\n", temppost->post_likes);
                temppost = temppost->next_post;
            }
            return;
        }
        temp = temp->next_user;
        k++;
    }
    printf("This username is not availble\n");
    return;
}

void like(user *head, int numofpost, char *loginname)
{

    int k = 0;
    int i = 0;
    char *username = (char *)malloc(sizeof(char));
    while ((username[i] = getchar()) != ' ')
    {
        i++;
        username = (char *)realloc(username, (i + 1) * sizeof(char));
    }
    username[i] = '\0';
    const char *postID = getchar();
    int pid = (int)(postID);
    pid = pid - 48;
    post *temppost = (post *)malloc(sizeof(post));
    user *temp = head;
    post *newnode = (post *)malloc(sizeof(post));
    if (newnode == NULL)
    {
        while (!newnode)
        {
            newnode = (post *)malloc(sizeof(post));
        }
    }
    while (numofpost > k)
    {
        if ((strcmp(temp->next_user->username, username)) == 0)
        {
            temppost = temp->next_user->post;
            if (pid == temppost->next_post->post_id)
            {
                for (int i = 0; temppost->next_post->people[i] != NULL; i++)
                {
                    if (strcmp(temppost->next_post->people[i], loginname) == 0)
                    {
                        printf("you liked this post before\n");
                        return;
                    }
                }
                temppost->next_post->post_likes = temppost->next_post->post_likes + 1;
                temppost->next_post->people = (char **)realloc(temppost->next_post->people, sizeof(char *) * (temppost->next_post->post_likes + 1));
                temppost->next_post->people[temppost->next_post->post_likes - 1] = loginname;
                temppost->next_post->people[temppost->next_post->post_likes] = NULL;
                printf("post liked\n");
                return;
            }
            temppost = temppost->next_post;
            k++;
        }
        else
        {
            temp = temp->next_user;
        }
    }
    printf("postID N/A\n");
    return;
}

int delete (user *headdelete, char *username)
{
    int post_id = 0;
    scanf("%d", &post_id);
    user *temp = headdelete;
    while (strcmp(temp->next_user->username, username) != 0)
    {
        temp = temp->next_user;
    }
    post *tempdelete = temp->next_user->post;
    while (1)
    {
        if (tempdelete->next_post->post_id == post_id)
        {
            printf("your post sucssfuly deleted\n");
            tempdelete->next_post = tempdelete->next_post->next_post;
            break;
        }
        tempdelete = tempdelete->next_post;
    }
}

int main()
{
    int count;
    int *postcount = 0;
    user *head = (user *)malloc(sizeof(user));
    int *login1 = (int *)malloc(sizeof(int));
    head->next_user = NULL;
    scan(head, count, &login1, &postcount);
}

void write_file(user *head)
{
    FILE *fp = fopen("accounts.txt", "w");
    FILE *f2p = fopen("post.txt", "w");
    user *temp = head->next_user;
    while (temp != NULL)
    {
        post *cur = temp->post->next_post;
        fprintf(fp, "%s", temp->username);
        fprintf(fp, "%s", temp->password);
        int count = 0;
        while (cur != NULL)
        {
            fprintf(f2p, "%s", cur->post_txt);
            fprintf(f2p, "%d\n", cur->post_likes);
            cur = cur->next_post;
            count++;
        }
        fprintf(fp, "%d\n", count);
        temp = temp->next_user;
    }
    fclose(fp);
    fclose(f2p);
}