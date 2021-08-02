#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>


#include "fs.h"
using namespace std;
// void duplicate(std::string fsrc, std::string fdest)
// {
//     std::ifstream src(fsrc, std::ios::binary);
//     std::ofstream dst(fdest, std::ios::binary);

//     dst << src.rdbuf();
// }

// TEST(FsTest, init021005){
//     initFs("fs2-10-5.bin", 2, 10, 5);
//     ASSERT_EQ(printSha256("fs2-10-5.bin"),std::string("F7:71:A2:19:63:85:52:25:AF:50:89:31:D7:BD:57:9E:BC:5E:3D:A2:85:4F:FE:41:B8:63:1A:5B:18:3F:0E:85"));
// }

// TEST(FsTest, init011010){
//     initFs("fs1-10-10.bin", 1, 10, 10);
//     ASSERT_EQ(printSha256("fs1-10-10.bin"),std::string("F4:ED:F3:23:45:16:CA:BF:78:1A:BE:6F:EF:DB:7F:0F:BA:07:F5:88:D7:A5:CD:65:1F:18:A4:81:65:91:E3:F4"));
// }

// TEST(FsTest, case3){
//     initFs("fs4-32-16.bin", 4, 32, 16);
//     ASSERT_EQ(printSha256("fs4-32-16.bin"),std::string("A2:71:21:00:D1:4C:10:94:C9:A0:0A:BD:03:E7:25:38:EA:3E:04:07:57:E4:02:87:5F:7D:1F:B7:35:6D:FE:E4"));
// }

// TEST(FsTest, case4){

//     duplicate("fs-case4.bin", "fs-case4.bin.back");

//     addFile("fs-case4.bin", "/teste.txt", "abc");
//     ASSERT_EQ(printSha256("fs-case4.bin"),std::string("AA:29:B7:CF:09:B6:32:0E:6B:20:51:ED:FD:8E:40:FB:B0:A8:71:FA:8A:22:0A:06:F4:E1:E4:69:0A:C6:B2:77"));

//     rename("fs-case4.bin.back", "fs-case4.bin");
// }

// TEST(FsTest, case5){
//     duplicate("fs-case5.bin", "fs-case5.bin.back");

//     addDir("fs-case5.bin", "/dec7556");
//     ASSERT_EQ(printSha256("fs-case5.bin"),std::string("0B:BB:60:5C:52:BC:0D:4F:5C:2C:B8:AA:2D:F5:F6:43:7A:EC:02:80:72:F2:D7:C3:7B:91:A6:FE:9E:4C:B6:44"));

//     rename("fs-case5.bin.back", "fs-case5.bin");
// }

// TEST(FsTest, case6){
//     duplicate("fs-case6.bin", "fs-case6.bin.back");

//     addFile("fs-case6.bin", "/dec7556/t2.txt", "fghi");
//     ASSERT_EQ(printSha256("fs-case6.bin"),std::string("C5:D5:15:D8:2F:09:15:49:D9:A2:B5:58:36:E7:DC:28:E5:C4:14:02:1D:03:0E:A8:4E:40:EE:76:BF:05:F0:C6"));

//     rename("fs-case6.bin.back", "fs-case6.bin");
// }

// TEST(FsTest, case7){
//     duplicate("fs-case7.bin", "fs-case7.bin.back");

//     remove("fs-case7.bin", "/dec7556/t2.txt");
//     ASSERT_EQ(printSha256("fs-case7.bin"),std::string("30:9A:B6:81:77:C1:FF:C9:7A:39:FD:77:DC:98:2A:47:C4:3A:31:A4:79:1A:49:FB:AB:8F:42:4B:66:7D:C0:8D"));

//     rename("fs-case7.bin.back", "fs-case7.bin");
// }

// TEST(FsTest, case8){
//     duplicate("fs-case8.bin", "fs-case8.bin.back");

//     remove("fs-case8.bin", "/dec7556");
//     ASSERT_EQ(printSha256("fs-case8.bin"),std::string("D4:63:6C:09:AD:B9:D3:68:6F:1B:02:79:78:38:50:C2:31:7D:E2:F1:C1:50:C9:13:7D:D9:0A:77:B5:27:4E:36"));

//     rename("fs-case8.bin.back", "fs-case8.bin");
// }

// TEST(FsTest, case9){
//     duplicate("fs-case9.bin", "fs-case9.bin.back");

//     move("fs-case9.bin", "/dec7556/t2.txt", "/t2.txt");
//     ASSERT_EQ(printSha256("fs-case9.bin"),std::string("DA:8A:BB:51:1A:83:6E:1B:CF:5E:EE:8F:13:53:FA:27:2C:A5:96:65:B8:50:E8:8E:16:64:90:D2:90:AD:92:39"));

//     rename("fs-case9.bin.back", "fs-case9.bin");
// }

// TEST(FsTest, case10){
//     duplicate("fs-case10.bin", "fs-case10.bin.back");

//     move("fs-case10.bin", "/teste.txt", "/dec7556/teste.txt");
//     ASSERT_EQ(printSha256("fs-case10.bin"),std::string("C5:D5:15:D8:2F:09:15:49:D9:A2:B5:58:36:E7:DC:28:E5:C4:14:02:1D:03:0E:A8:4E:40:EE:76:BF:05:F0:C6"));

//     rename("fs-case10.bin.back", "fs-case10.bin");
// }

// TEST(FsTest, case11){
//     duplicate("fs-case11.bin", "fs-case11.bin.back");

//     remove("fs-case11.bin", "/a.txt");
//     ASSERT_EQ(printSha256("fs-case11.bin"),std::string("B6:0E:AE:15:59:C4:CE:26:44:AD:17:20:62:29:98:F3:AC:D5:BD:24:EE:46:B8:CF:3C:A0:77:A2:5E:4B:D0:44"));

//     rename("fs-case11.bin.back", "fs-case11.bin");
// }

// TEST(FsTest, case12){
//     duplicate("fs-case12.bin", "fs-case12.bin.back");

//     move("fs-case12.bin", "/b.txt", "/a.txt");
//     ASSERT_EQ(printSha256("fs-case12.bin"),std::string("F3:B1:7D:6A:D9:BF:24:4B:CA:97:F9:1F:EB:63:DE:6D:CE:93:89:08:BC:DA:13:41:8C:A4:FE:39:78:1B:6A:AE"));

//     rename("fs-case12.bin.back", "fs-case12.bin");
// }

int main(int argc, char **argv)
{
    //   testing::InitGoogleTest(&argc, argv);
    //   return RUN_ALL_TESTS();
    // initFs("fs-meu.bin", 2, 8, 6);
    addFile("fs-meu.bin", "/teste.txt", "abc");
    return 0;
}

// achar inode não utilizado, então coloca os daos do inode (nome do arquivo, tamanho dele)
// ai vai ter que encontrar os bloc( sai varendo o mapa de bit até encontrar o nulo, então seta o bit como utilizado, vai n oinode e coloca o conteudo na região de blocos)
// precisa atualiza o pai do Inode (Vai no dir pai e incrmenta o tamanho dele, e vai na região de blocos e coloca o indice do inode que inclui no novo arquivo) 
// Olhar o slide 3, tem um byte que é separado (como é enumerado)