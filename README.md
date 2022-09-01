<h1>Descripción</h1>
Stockholm es un malware, específicamente un ransomware hecho en C que afecta a sistemas basados en Linux. Cuidado a la hora de usarlo ya que el programa encriptará archivos. Todos los archivos afectados por <a href="https://es.wikipedia.org/wiki/WannaCry">WannaCry</a> serán encriptados mediante <a href="https://es.wikipedia.org/wiki/Advanced_Encryption_Standard">AES-256</a>, agregando la extensión .ft a los archivos

<h1>Uso</h1>
<ul>
<li>Para que funcione correctamente deberemos modificar los path de las lineas 216 y 405.</li>
<li>Tras hacer esto debemos compilar el programa de la siguiente manera: gcc stockholm.c -o stokholm.</li>
<li>Una vez compilado el archivo deberá estar en la carpeta que queramos infectar.</li>
</ul>
<p>Se ejecutará como cualquier programa, de la siguiente forma: ./stockholm</p>

<h1>Flags</h1>
El programa cuenta con una serie de flags que podremos utilizar

```
▶ -h / -help: cualquiera de estos flags como argumento (solo uno) mostrará un mensaje de ayuda que explica el funcionamiento del programa.</br>
▶ -v / -version: cualquiera de estos flags como argumento (solo uno) mostrará la versión actual del programa</br>
▶ -s / -silent: cualquiera de estos flags como argumento (solo uno) evitará que el programa muestre ningún tipo de output</br>
▶ -r / -reverse: cualquiera de estos flags como argumento (solo uno) seguido de la key mostrada al ejecutar el programa hará una desinfección de los archivos infectados (desencriptado)</br>
```

<h2>Uso de las flags</h2>
<p>Las flags se podrán usar de la siguiente forma:</p>
<ul>
<li>./stockholm -h (esto mostrará el menú de ayuda).</li>
<li>./stockholm -v (esto mostrará la versión del programa).</li>
<li>./stockholm -s (esto hará el encriptado sin mostrar nada por pantalla, habrá que leer el archivo .stockholm.txt para ver la contraseña de desencriptado).</li>
<li>./stockholm -r <key> (esto desencriptará los archivos si la key es válida).</li>
<li>./stockholm -s -r <key> (esto desencriptará los archivos si la key es válida sin mostrar nada por pantalla).</li>
</ul>

El programa esta basado en el proyecto Stockholm de <a href="https://github.com/gemartin99">gemartin99</a>.
