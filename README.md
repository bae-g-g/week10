# 유튜브링크
- 작동설명
https://youtu.be/zwd3GKvCSxg?si=nDjShfThVRDAVZ3A
- 코드설명
https://youtu.be/77Cm22JIbjk?si=3-wCEzwBd5EIzSng

# 회로설명
![스크린샷 2025-05-11 오후 11 54 08](https://github.com/user-attachments/assets/ac848158-1b99-4c2c-8eef-9a3aa3b092f6)
![IMG_5317](https://github.com/user-attachments/assets/e27905e7-0103-45ad-bcc9-b89b00351d89)

### R9DS <-> 아두이노 
- 3번 채널 신호선 <-> A1
- 5번 채널 신호선 <-> A2
- 6번 채널 신호선 <-> A0

### 3색 LED <-> 아두이노
- RED <-> 3
- GREEN <-> 5
- BlUE <-> 6

# RadioLink AT9
![image](https://github.com/user-attachments/assets/6dbeec2c-0ac7-4e54-be33-915906aa9b16)


- 3번 채널 펄스폭  1300 ~ 1950 조절가능
- 5번 채널 펄스폭  1200,1550,1900 세 개
- 6번 채널 펄스폭  1150,1950 두 개

 # 동작
## 펄스폭 계산

![image](https://github.com/user-attachments/assets/40c39fa2-6eb4-4869-a279-0bd8cdd76b0c)
- 신호 LOW -> HIGH :
  인터럽트가 걸리고 현재시간 저장
- 신호 HIGH -> LOW :
   다시 인터럽트가 걸리고 현재시간과 이전 인터럽트에서 저장했던 시간의 차이로 펄스폭 계산


## 과제동작
1. LED On/Off :
  6번 채널을 이용해 펄스폭이 1950인 경우 ON, 1150인 경우 OFF
   
2. LED 밝기 제어 :
   3번 채널 이용해 펄스폭을 0~255로 매핑시켜 그 값을 밝기로 사용

3. 3색 LED 색 변환 : 
   5번 채널 사용 펄스폭이 1200=RED,1550=GREEN,1900=BLUE로 선택    
