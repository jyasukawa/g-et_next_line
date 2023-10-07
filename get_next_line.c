#include "get_next_line.h"

static char	*ft_read_until(int fd, char *buffer, int *is_error)
{
	char		*tmp;
	ssize_t	bytes_read;

	tmp = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (tmp == NULL)
		return (ft_error_found(buffer, is_error));
	bytes_read = 1;
	while (ft_strchr_null(buffer, '\n') == NULL && bytes_read != 0)
	{
		bytes_read = read(fd, tmp, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(tmp);
			return (ft_error_found(buffer, is_error));
		}
		tmp[bytes_read] = '\0';
		buffer = ft_strjoin_free(buffer, tmp, is_error);
		if (buffer == NULL)
		{
			free(tmp);
			return (ft_error_found(buffer, is_error));
		}
	}
	free(tmp);
	return (buffer);
}

static char	*ft_clip_line(char *buffer, int *is_error)
{
	size_t	i;
	char		*str;

	if (buffer[0] == '\0')
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	str = (char *)malloc(sizeof(char) * (i + 2));
	if (str == NULL)
		return (ft_error_found(buffer, is_error));
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		str[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		str[i] = buffer[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	*ft_save_remainder(char *buffer, int *is_error)
{
	size_t	i;
	size_t	j;
	char		*str;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\0')
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - i));
	if (str == NULL)
		return (ft_error_found(buffer, is_error));
	i++;
	j = 0;
	while (buffer[i])
		str[j++] = buffer[i++];
	str[j] = '\0';
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char				*nextline;
	int					is_error;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	is_error = NO;
	buffer = ft_read_until(fd, buffer, &is_error);
	if (is_error == YES)
		return (NULL);
	nextline = ft_clip_line(buffer, &is_error);
	if (is_error == YES)
		return (NULL);
	buffer = ft_save_remainder(buffer, &is_error);
	if (is_error == YES)
		return (NULL);
	return (nextline);
}

// ＜メモ＞
// Joinで改行まで、やjoinのなかで初期化、などのやり方もとうぜんあるが、関数を機能ごとにわけて一つの関数に一つの機能
// nextlineのfreeは使い手次第

// ft_strchr_null関数のif(c == ‘\0’)など、必要のない部分は省略した
// ft_strjoin_free のtmpのnullチェックも要らない
// 変数が-1をとるためデータ型はssize_t

// ft_clip_line
// の手前でbuffer==NULLの場合は弾かれるため、この時点でbufferは３パターン
// １，通常の改行込
// ２，改行なしの最後の１行
// ３，読み込むものがない空文字

// ft_save_remainder
// この時点ではbufferは
// １，通常の改行込
// ２，改行なしの最後の１行

// buffer[0] == '\0' の状態の buffer は、空の文字列を表すポインタです。C言語の free 関数は、メモリの動的解放を行うために使用されますが、正確なポインタを渡さなければなりません。buffer[0] == '\0' の場合、buffer はポインタとして有効ですが、実際にメモリが割り当てられていないため、free 関数に渡すことはできません。buffer[1] == ‘\0’なら可

// ft_save_remiderはNULLが帰ってきてもNULLをsaveして次回以降にその内容を引き継ぐ

// OPEN_MAXの定数はubuntu環境だと廃止されていること。 感想 OPEN_MAXを使わず、_SC_OPEN_MAXの方がいいかも
// Testerでエラーになる

// 要するに、fd_max は個々のプロセスが保持できるファイルディスクリプタの数を制限し、open_max はシステム全体で同時にオープンできるファイルディスクリプタの数を制限します。通常、fd_max は open_max よりも小さい値である必要があります。

// read関数の使い方
// #include <unistd.h>
// ssize_t read(int fd, void *buf, size_t count);
// 引数
// fd	読み出すデータが格納されているファイルディスクリプタ（ファイル記述子）
// buf	読み出したデータを格納するバッファの先頭アドレス
// count	読み込む最大バイト数
// 戻り値
// 正の値	読み込んだバイト数
// 0	ファイルオフセットが既にEOF
// -1	エラーが発生 (errno にエラー番号がセットされています)
// 代表的なエラー (errno)
// * EAGAINfd がソケットや PIPE 以外を指していて O_NONBLOCK が設定されており、読み込みを行うと停止する状態にある
// * EBADFfd が有効なファイルディスクリプターでないか、読み込みのために オープン (open) されていない
// * EINTR指定されたバイト数を読み込み完了するまでにシグナルに割り込まれた

// 機能
// * ファイルディスクリプター fd から最大 count バイトをアドレス buf で始まるバッファーへ読み込む
// * fd が seek に対応しているファイルを指している場合、read は現在のファイルオフセットから開始される。ファイルオフセットは読み込んだバイト数分だけインクリメントされる
// * 現在のファイルオフセットがファイル末尾の場合は、読み出しは行われず 0 を返す
// * count が SSIZE_MAX より大きい場合、動作は保証されない

// 注意すること
// 注意ポイント①
// read 関数は、引数 count で指定したバイト数を必ず読み込んでくれるわけではない。
// 例えば、read 処理中に signal などの割り込みが発生した場合、 read は途中で中断されます。これらのケアを正しく行わないと、中途半端なデータを処理してしまってプログラムが破綻してしまう。
//  
// 注意ポイント②
// ソケットなどにアクセスする場合は、届いているデータを最後まで読み出す必要があります。
// カーネル側のバッファが溢れ、書き込む側でエラーが発生します。


// 【書式】int read(int handle, void *buf, unsigned n);
// 【パラメータの型と説明】
// パラメータ	説明
// int handle	ファイルハンドル
// void *buf	データを格納するアドレス
// unsigned n	データの大きさ
// 【機能説明】
// handleにつながっているファイルからnバイト読み込みbufに格納します。
// ファイルハンドルは、ファイルディスクリプタとも呼ばれます。
// 読み込みに成功したときは読み込んだバイト数を、失敗したときは-1を返します。
// #include <stdio.h>
// #include <fcntl.h>

// main()
// {
//   int     fd;
//   char    buff[100+1];
//   int     rc;

//   fd = open("test.txt", O_RDONLY);
//   if (fd == -1)
//   {
//           printf("ファイルオープンエラー\n");
//           return;
//   }
//   rc = read(fd, buff, 100);
//   if (rc == -1)
//   {
//           printf("ファイル読み込みエラー\n");
//   }
//   else
//   {
//           buff[rc] = '\0';
//           printf("read:%d - %s\n", rc, buff);
//   }
//   close(fd);
// }

// 「buff[rc] = '\0';」でread()関数で読み込んだ配列の最後に'\0'を書き込む必要があります。 文字列の最後には'\0'が必要なためです。
// これを行わないとprintf()関数などで文字列として表示できません。
// また、配列のサイズが「100+1」なのはread()関数が100バイトフルで読み込んだときに'\0'を書き込む位置が必要なためです。

// You will compile your code as follows (a buffer size of 42 is used as an example): cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 .c

// * BUFFER_SIZE は、読み取り操作時に一度に読み込むバイト数を指定します。つまり、この関数はファイルからデータを読み取り、指定したバッファーサイズ（BUFFER_SIZE）ごとにテキストを読み込みます。
// * 例えば、BUFFER_SIZE が 32 の場合、この関数は最大で 32 バイトのテキストデータを 1 回の読み取り操作で読み込みます。もし行の途中で 32 バイトに達した場合でも、読み取りはそこで一時停止し、次回の読み取り操作に残りのテキストが読み込まれます。
// * BUFFER_SIZE の適切な値は、特定の使用ケースに依存します。大きすぎると、メモリの無駄遣いになり、小さすぎると効率が低下します。適切な値を選ぶためには、プロジェクトの要件や性能に関する考慮が必要です。