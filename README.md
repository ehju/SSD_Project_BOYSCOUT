# SSD_Project_BOYSCOUT
## BOYSCOUT Rule
### Pull 할 때보다, 더 깨끗한 코드를 Push 한다면코드는 나빠질 일이 없다. -> 지속적으로 코드가 개선 된다

## Members
이승현, 주은혜, 정진섭, 허훈, 정혜진, 박세운(Leader) 

## Ground Rules
<Ground Rule 3주-2일차 생성> 

📌 Pull Request(PR) 규칙
##### 1. PR은 feature 브랜치에서만 생성한다.
##### 2. PR 작성 전 빌드 성공 여부를 반드시 확인한다.
##### 3. PR은 최소 1인 이상 Approval을 받아야 한다.
##### 4. PR 설명은 변경사항 위주로 최소 2줄 이상 작성한다.
##### 5. Patch commit 메시지는 1줄로 간결하게 작성한다.

📌 Conflict 발생 시 처리 방식
##### 6. GitHub 웹에서 충돌 해결을 금지한다.
##### 7. 해당 브랜치 Owner가 로컬에서 직접 충돌을 해결해야 한다.
##### 8. 로컬에서 정상 빌드 및 동작을 확인한 후 PR을 생성한다.

📌 시간 기반 작업 규칙
##### 9. 매일 9시 30분, 14시에 데일리 스크럼 진행
##### 10. 각 회의는 10분 이내로 진행
##### 11. 16시 30분 이후 Merge 금지
##### 12. 17시 퇴근 이후에는 장소에 관계없이 자유롭게

📌 Commit Message 규칙
##### 13. 모든 커밋 메시지에 Prefix를 사용해야 한다.
###### - add: 신규 기능 추가 ex) [add] new TC1 [add] new implementation for TC1
###### - fix: 마이너 수정 ex) [fix] build fix for TC1
###### - refactor: 리팩토링 ex) [refactor] clean for TC1


## 코드리뷰 잘하기 위한 좋은방향 - BoyScout
(우리팀만의 바람직한 코드 리뷰 전략 모색)
1. 사전 구두 합의된 항목은 PR에 명시하고 신속히 Merge
2. Pull Request용 템플릿(What/Why) 사용하기
3. 쿠션어는 마음 가득 전달했으니 쿨하게 생략하자
4. PR단위는 feature/refactor 단위로 하자
5. PR리뷰 30분 이내 목표이고, 리뷰어가 없으면 팀장이 로테이션으로 지정
6. 코드리뷰는 전원이 참여하도록 하되 본인개발속도에 맞게 skip 가능
7. PR 올린순서대로 Review -> Merge 하기
8. 팀원 전체에게 공유해야 하는 commit은 해결즉시 구두로 알리고 commit하기

## PR Template
### Changes : What(feature/bugfix) -> Why(optional)
- 안쓰는 func.삭제 및 Masic 넘버 수정.
### Which solution (Mandatory)
- [ ] SSD
- [ ] Logger
- [ ] TestShell / TestScenario
### To Reviewer(optional)
- 리뷰할때 미리 알아야 할 내용이 있다면 기입해주세요.
  
## BOYSCOUT SSD Program Help
### TEAM_NAME: BOYSCOUT
### TEAM_LEADER: 박세운
### TEAM_MEMBER: 이승현/주은혜/정진섭/허훈/정혜진
write <LBA> <VALUE>: LBA = 0 ~ 99 , VALUE = 0x00000000 ~ 0xFFFFFFFF(10 Digit) // Write Value @LBA 
read <LBA>              : LBA = 0 ~ 99 // Read @LBA

erase <LBA> <SIZE>      : LBA = 0 ~ 99 , SIZE = (+/-Decimal) // Erase Value @LBA ~@LBA+SIZE

erase_range <START_LBA> <END_LBA>: LBA = 0~99 // Erase @ STARTLBA~ENDLBA

fullwrite <VALUE>       : VALUE = 0x00000000 ~ 0xFFFFFFFF(10 Digit) // Write Value @ALL LBA

fullread                : No Param //Read Full Range

flush                   : No Param //Flush Buffer

1_FullWriteAndReadCompare : No Param //Write and Read Compare @ AllRange

1_              : No Param //Write and Read Compare @ AllRange

2_PartialLBAWrite : No Param //(Write 0x12345678 @LBA_0 ~ 4 & ReadCompare) * 30 times

2_              : No Param //(Write 0x12345678++ @LBA_0 ~ 4 & ReadCompare) * 30 times

3_WriteReadAging : No Param //(Write RandomValue @LBA_9 and @LBA_99) * 200 times

3_              : No Param //(Write RandomValue @LBA_9 and @LBA_99) * 200 times

4_EraseAndWriteAging : No Param //(Write/OverWrite/Erase)* 30 times

4_              : No Param //(Write/OverWrite/Erase)* 30 times

exit            : No Param //Terminate Shell

help            : No Param //Print Command Usage
