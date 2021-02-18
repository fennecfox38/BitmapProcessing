# [BITMAP Project](https://fennecfox38.github.io/2021/01/31/Bitmap-Project.html)

이 repository는 프로젝트를 진행하고 velog에 포스팅하며 기록으로 남겨놓기 위해 만들었습니다.
BITMAP Project는 

- toASCII: 비트맵 파일을 아스키 아트 텍스트 파일로 변환
- contrast: 색상 차를 증가시켜 대비를 강조하는 변환
- gray: 흑백(grayscale)로 변환
- invert: 색상반전
- mirror: 좌우반전

을 지원합니다. 연습용으로 만들어 **그 품질은 조악합니다.**

입력으로는 .bmp 확장자의 비트맵 파일중,
- BITMAPINFOHEADER DIB 헤더를 사용하며, (V4HEADER,V5HEADER 미지원)
- 색 깊이 8bit (RLE 8 미지원)
- 색 깊이 16bit (555, 565 지원 / 알파채널 미지원)
- 색 깊이 24bit
- 색 깊이 32bit (알파채널 미지원)

만 지원합니다.

이 repository의 내용들은 VS Code와 C/C++ Extension, mingw64 gcc, c11 환경에서 작성되었으며,
참고를 위해 source, result폴더의 자료들, .vscode 아래의 json들, Makefile을 남겨놓았습니다.

사용 방법

    (project directory)>./bmputil [option] [src filename (w/ extension)] [dst filename (w/ extension)]
    option: [ toASCII: -a | contrast: -c | gray: -g | invert: -i | mirror -m ]

자세한 설명은 [fennecfox38.github.io의 bitmap 시리즈](https://fennecfox38.github.io/2021/01/31/Bitmap-Project.html)를 참조하십시오.

ex)


![pepper_24](/source/pepper/24.bmp) 원본

![pepper_24_contrast](/result/pepper/24_contrast.bmp) 색상대비

![pepper_24_gray](/result/pepper/24_gray.bmp) 흑백

![pepper_24_invert](/result/pepper/24_invert.bmp) 색상반전

![pepper_24_mirror](/result/pepper/24_mirror.bmp) 좌우반전
