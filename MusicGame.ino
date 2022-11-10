
#include "Defination.h"
#include "Song.h"
#include "SSD1306Wire.h"
#include "Vector.h"


#define	IMPLEMENTATION	LIFO



class StateController;
class IState;
class IAphlaBatPoint;


bool FormerPress( IAphlaBatPoint*  AI  ,  IAphlaBatPoint*  AI2);
/* 基本場景宣告*/

class IAphlaBatPoint{
  
  


public:


  int16_t _PointX;
  int16_t _PointY;
  bool IsPrssppL = false;
  bool IsPrssppR = false;
   bool Prefect = false;
  bool Good = false;
  bool Normal = false;
  bool Bad = false;
  bool Miss = false;
  bool ISHit = false;
  int Remove = 0;
  


  virtual void draw(){}
  virtual float Hit( IAphlaBatPoint* Later ){
      
      Prefect = false;
      Good = false;
      Normal = false;
      Bad = false;
      Miss = false;
      float score = 0;
 
    if(_PointX > _IAphlaBatPointDefine.HitPos- (_IAphlaBatPointDefine.Aphlabat_radius*4)  &&_PointX < _IAphlaBatPointDefine.HitPos+ (_IAphlaBatPointDefine.Aphlabat_radius*6)   ){
   
        Remove = 1;
        ISHit = true;
        score = _IAphlaBatPointDefine.Score;
          if( _PointX <= _IAphlaBatPointDefine.HitPos + _IAphlaBatPointDefine.HitPosTolirance*2 &&  _PointX >= _IAphlaBatPointDefine.HitPos - _IAphlaBatPointDefine.HitPosTolirance*2  ){

                score *= _IAphlaBatPointDefine.Score_Prefect;
              /*Perfect 的範圍 */
              Prefect =true;
          }
          else if( _PointX <= _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*4) &&  _PointX >= _IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*4)  ){

            /*Good 的範圍 */
            score *= _IAphlaBatPointDefine.Score_Good;
            Good = true;

          }
          else if(_PointX <= _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*6 ) &&  _PointX >=_IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*6) ){
            /*Normal 的範圍 */

            score *= _IAphlaBatPointDefine.Score_Normal;

            Normal = true;
          }
          else if(_PointX <=  _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*8 ) &&  _PointX >= _IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*8 )){
            /*bad 的範圍 */

            score *= _IAphlaBatPointDefine.Score_Bad;
            Bad  = true;
          }
          else{
            Remove = 3;
            score= 1;
            Miss =true;
          }
   

    }
    else{
       
       score = 0;
    }
  


    return score;

  }
  virtual bool offest(float& Score , IAphlaBatPoint* Later){ return false;}
  IAphlaBatPoint(int16_t PointX , int16_t PointY){_PointX = PointX ; _PointY  = PointY; ISHit = false; }
  IAphlaBatPoint(){_PointX = 128 ; _PointY  =32;ISHit = false;}


};



/*實心大音符*/
class FillCircle : public IAphlaBatPoint{


public:
  FillCircle(int16_t PointX , int16_t PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

   
  bool offest(float& Score , IAphlaBatPoint* Later) override {
    int Temp = 0;
     this->_PointX-=_IAphlaBatPointDefine.OffestSpeed;

    if( digitalRead(LeftHit) == HIGH && (!IsPrssppL)){
      
        Temp = this->Hit(Later);
        Score+=Temp ;

          
        IsPrssppL = true;

    }
    if( digitalRead(LeftHit) == LOW && (IsPrssppL)){

        IsPrssppL = false;

    }

    return Temp;

  }


   virtual void draw() override {

    display.fillCircle( _PointX , _PointY , _IAphlaBatPointDefine.Aphlabat_radius);

  }

};


class HollowCircle: public IAphlaBatPoint{

public:

  HollowCircle(int16_t PointX , int16_t PointY) : IAphlaBatPoint(PointX , PointY){

  }


  bool offest(float& Score , IAphlaBatPoint* Later) override {

      int Temp = 0;
     _PointX-=_IAphlaBatPointDefine.OffestSpeed;

    if( digitalRead(RightHit) == HIGH && (!IsPrssppR)){

       Temp = this->Hit(Later);
        Score+=Temp ;

        IsPrssppR = true;

    }
    if( digitalRead(RightHit) == LOW && (IsPrssppR)){

        IsPrssppR = false;

    }

    return Temp;

  }

  virtual void draw() override {

    display.drawCircle( _PointX , _PointY , _IAphlaBatPointDefine.Aphlabat_radius);

  }

};




class FillComboHit : public IAphlaBatPoint{


public:
  FillComboHit(int16_t PointX , int16_t PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

  float Hit( IAphlaBatPoint* Later  ){
      float score = 0;
      if(_PointX <= _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*4) &&  _PointX >= _IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*4)  ){

        score += _IAphlaBatPointDefine.Score*_IAphlaBatPointDefine.Score_Good;
        Prefect = true;

      }
      else{
        
        score = 0;
      }

      return score;

  }
  bool offest(float& Score , IAphlaBatPoint* Later) override {

     _PointX-=_IAphlaBatPointDefine.OffestSpeed;

    if( digitalRead(RightHit) == HIGH && (!IsPrssppR)){

        Score+=this->Hit(Later);
        IsPrssppR = true;

    }
    if( digitalRead(RightHit) == LOW && (IsPrssppR)){

        IsPrssppR = false;

    }

    if( digitalRead(LeftHit) == HIGH && (!IsPrssppL)){

        Score+=this->Hit(Later);
        IsPrssppL = true;

    }
    if( digitalRead(LeftHit) == LOW && (IsPrssppL)){

        IsPrssppL = false;

    }
    
    return false;

  }


  virtual void draw() override {

    display.fillCircle( _PointX , _PointY , _IAphlaBatPointDefine.Aphlabat_radius*2.3f);

  }

};

  Vector<IAphlaBatPoint*> V0  ;

class IState{

public: 
  String _state_name = String("NULL");
  

protected:

  
  StateController* _stateController = nullptr;

public:
  IState(){}
  IState(StateController* stateController );   /*傳入StateController                                                                   這樣才能藉由Controller
                                                                                       更新State 
                                                                                        */
  virtual void StateBegin(){}
  virtual void StateUpdate(){}
  virtual void StateEnd(){}
};


/* Start場景宣告定義*/

class StartState :public IState {

  
public:

  /*屬性宣告*/
  unsigned long CurrentTime;
  unsigned long LastTime;
  
  

  /*繼承覆寫*/
  
  virtual void StateBegin() override {
    CurrentTime = millis();
    LastTime = CurrentTime ;

    display.clear();

    display.setFont(  _StartStateDefine.WeclomeTextFont );
    display.drawString(  _StartStateDefine.WeclomeText_X , _StartStateDefine.WeclomeText_Y, _StartStateDefine.WeclomeText    );
    display.setFont(  _StartStateDefine.SubTextFont );
    display.drawString(  _StartStateDefine.SubText_X , _StartStateDefine.SubText_Y, _StartStateDefine.SubText    );
    display.display();

    



  }

   virtual void StateUpdate() override ;
  
  
   virtual void StateEnd() override ;


  /*自己的method*/

  /*建構*/
  StartState( StateController* stateController ):IState(stateController){

    _state_name = "StartState" ;
    Serial.println("Change!!");

  }


  
};

/* MainMenuState場景宣告定義*/

class MainMenuState :public IState {

public:

  /*屬性宣告*/

  int _index;
  int16_t cursor_pos_Y ; 
  bool IsPressR ;
  bool IsPressL;
  int Page ;
  int temp_i = 0;

  /*繼承覆寫*/
  
  virtual void StateBegin() override {


    _index = 0;
    cursor_pos_Y = 0 ;

    display.clear();
    IsPressR = false;
    IsPressL = false;
    Page = 1;


    
  }

   virtual void StateUpdate()  override;

   
      
   virtual void StateEnd() override ;





  /*自己的method*/
   MainMenuState( StateController* stateController ):IState(stateController){

    _state_name = "MainMenuState" ;
    Serial.println("Change!!");

  }

private:

  int BtnChoice( int index) {

      if( digitalRead(RightHit) == HIGH  &&  (! IsPressR) ){

          index+=1;
          if(index >= _MainMenuStateDefine.SongArray ){
            index = 0;
          }

          IsPressR = true;

      }

      if(digitalRead(RightHit) == LOW  &&  (IsPressR)){
        IsPressR = false;
      }



      if( digitalRead(LeftHit) == HIGH  &&  (! IsPressL) ){

          index-=1;
          if(index < 0 ){
            index =  _MainMenuStateDefine.SongArray-1;
          }
          IsPressL = true;

      }

      if(digitalRead(LeftHit) == LOW  &&  (IsPressL)){
        IsPressL = false;
      }
    
    Page = (index /4) +1;
    return index;

  }

  void CursorOffset(){

    cursor_pos_Y = (_index%4)* _MainMenuStateDefine.Cursor_offest_Y+2 ;

  }


  void Draw(){

      display.clear();

      display.setFont( _MainMenuStateDefine.SongNameFont );
      
      for(int i = 0  ; i < SongArrayLength ; i+=1){

          if( Page > 1 ){

              if( i < (Page-1)*4 || i >= (Page)*4  ){
                continue;
              }

          }
          else{

              if(i>4){
                continue;
              }
          }

            
          display.drawString( _MainMenuStateDefine.SongNameTextStart_X , i * _MainMenuStateDefine.Cursor_offest_Y   , _MainMenuStateDefine.SongName[i] );
          

      }
      
      display.fillCircle( _MainMenuStateDefine.Cursor_X , cursor_pos_Y+6 , _MainMenuStateDefine.Cursor_Radius  );
      
      display.display();
  }

};
  IAphlaBatPoint Pont  ;

class MusicState :public IState {

public:

  /*屬性宣告*/


  int _index;
 
  int CurrentTime;
  int LastTime;
  int CurrentTimeHit;
  int LastTimeHit;
  int TextTimeCurrent;
  int TextTimeLast;
  int OffestCurrentTime ;

  int _SongCurrentTime ;
  int _SongLastTime ;
  int OffestLastTime ;
  unsigned long _SongStartTime;

  unsigned long _SongIndex ;
  unsigned long _SongIndexBuzzer;
  unsigned long _SongLength;
  unsigned long _SongBPM;
   int* _SongBGM ;
  String _HitPoint;
  bool Prefect = false;
  bool Good = false;
  bool Normal = false;
  bool Bad = false;
  bool Miss = false;
  bool IsTemp ;

  float Score;

  unsigned long MusicIndex;
  bool PlayB = true;

  
  /*繼承覆寫*/


  virtual  void StateBegin() override {

    _SongBPM = SongBPM[_index];
    _SongLength =SongLength[_index];
    Serial.println(_SongLength);
    _HitPoint = HitPoint[_index];


    CurrentTime = millis();
    CurrentTimeHit = millis();
    _SongCurrentTime = millis();
    TextTimeCurrent = millis();
    OffestCurrentTime = millis();
    LastTime = CurrentTime;
    LastTimeHit = CurrentTimeHit;
    _SongLastTime =  _SongCurrentTime ;
    TextTimeLast= TextTimeCurrent;
    OffestLastTime = OffestCurrentTime;
    _SongStartTime = SongStartTime[_index];
    Score = 0;
    _SongIndex  = 0;
    _SongIndexBuzzer = 0;
    _SongBGM = new int[_SongLength];

    _SongBGM = SongBGM[_index]  ;
    V0.Clear();

  }

   virtual void StateUpdate()  override;

   
      
   virtual void StateEnd() override ;



  /*自己的method*/
  MusicState( StateController* stateController , int index ):IState(stateController){

    _state_name = "MusicState" ;
    _index = index;
    Serial.println("Change!!");

  }
  
  void PlayMusic(){





        
      if(CurrentTime - LastTime >= SongBGM_Delay[_index][_SongIndexBuzzer]   ){
        
        ledcWriteTone(Channel, _SongBGM[_SongIndexBuzzer]); 

          _SongIndexBuzzer+=1;
      }

  }

  
  void DrawBack(){
    display.drawVerticalLine(_IAphlaBatPointDefine.HitPos , 0 , 62);
  }

  void SpawnHitPoint(){

    if(PlayB){

      IAphlaBatPoint*  Temp =   String2Aphlabat (_HitPoint);

      if(Temp != nullptr ){
           V0.PushBack( Temp );
      }

      PlayB  = false;

    }

    if(CurrentTimeHit  - LastTimeHit  >= (5000.0f/SongBPM[_index] )  && (!PlayB) ){

      
      _SongIndex+=1;
      PlayB = true;

      LastTimeHit = CurrentTimeHit;
   }
    

  }


    void Destroy(int i){
      V0.Erase(i );
   
  }

  void Draw(){

      
      
        SpawnHitPoint();

      if(OffestCurrentTime - OffestLastTime >= 16 ){

      for(int i = 0 ; i <V0.Size(); i++){

         
          
          IsTemp = V0[i]->offest(Score , V0[i]);

          if(i>0){
            V0[i]->ISHit  =false;
          }

        if(IsTemp){
            Pont = *(V0[0]);
        }


          
          V0[i]->draw();
          

           

       
        if((V0[i]->_PointX <= 4)){
          Destroy(i);
        }

        OffestLastTime = OffestCurrentTime;
         
      }

       
         



      if(V0.Size()>0){

        

        if(V0[0]->ISHit ){
            Pont = *(V0[0]);
            Destroy(0);
       }
        
    }
       
    }   

       if(Pont.Prefect ){
              display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Perfect");

            
          }
          else if(Pont.Good){
           
               display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Good");
          }
          else if(Pont.Normal){
            
             display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Normal");

            
          }
          else if(Pont.Bad){
             display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Bad");

          }
          else if(Pont.Miss){
               display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Miss");


         }


        



       if(TextTimeCurrent - TextTimeLast > 2000){
            Pont.Bad = false;
            Pont.Miss = false;
            Pont.Normal = false;
            Pont.Good = false;
            Pont.Prefect = false;
             TextTimeLast =TextTimeCurrent;
        }
         



       
}


  IAphlaBatPoint* String2Aphlabat(String HitPointString){

      switch (HitPointString[ _SongIndex ])
      {
      case '1':
        return new FillCircle( _IAphlaBatPointDefine.HitPosStart_X , _IAphlaBatPointDefine.HitPosStart_Y  );
        break;
      case '2':
        return  new HollowCircle(_IAphlaBatPointDefine.HitPosStart_X ,_IAphlaBatPointDefine.HitPosStart_Y);
        break;
      case '8':
       return  new FillComboHit(_IAphlaBatPointDefine.HitPosStart_X ,_IAphlaBatPointDefine.HitPosStart_Y);
        break;
      case '0':
        return nullptr;
        break;
      }

     return nullptr;
      
  } 

  

};


class CalculateState :public IState {

  
public:

  /*屬性宣告*/
  unsigned long CurrentTime;
  unsigned long LastTime;
  float _Score;
   int16_t _index;

  /*繼承覆寫*/
  
  virtual void StateBegin() override {
    CurrentTime = millis();
    LastTime = CurrentTime ;



    display.clear();

    display.setFont(  _ClaCulateStateDefine.TextFont);
    display.drawString(  _ClaCulateStateDefine.Text_X, _ClaCulateStateDefine.Text_Y, String(_Score)   );
   // display.setFont(  _StartStateDefine.SubTextFont );
    //display.drawString(  _ClaCulateStateDefine.SubText_X , _ClaCulateStateDefine.SubText_Y,   );
    display.display();

    



  }

   virtual void StateUpdate() override;

   virtual void StateEnd() override;

  /*自己的method*/

  /*建構*/
  CalculateState( StateController* stateController , float Score , int index ):IState(stateController){
    _index = index;
    _Score  = Score ;
    _state_name = "CalculateState" ;
    Serial.println("Change!!");

  }
  
};


/* 控制場景 */
class StateController{

public:
  IState* _state = nullptr;
  bool StateFirstRun = true;

public:

   virtual void SetState(IState* state , String state_name ){   /* 讓繼承IState的其他State做更新，換成新的State*/

    if(_state != nullptr){
      _state->StateEnd();
    }

    
    Serial.println(state_name);
    _state = state;
    
  }

   virtual void StateUpdate(){

      if( _state != nullptr && StateFirstRun ){
        _state->StateBegin();
        StateFirstRun = false;
      }

    if( _state != nullptr ){
     
      _state->StateUpdate();      /* 目前的State的Update，_state.StateUpdate()等於就是
                                   遊戲每禎更新

                                 */
    }
     
  }
  
  
};

/*Game Loop*/
StateController* stateController;

void setup() {


  Serial.begin(115200);
  // put your setup code here, to run once:
   stateController = new StateController();

  ledcSetup(Channel , Res ,  fqz);
  ledcAttachPin(BuzzerPin,  Channel );
  pinMode(RightHit , INPUT);
  pinMode(LeftHit , INPUT);
  pinMode(ConfirmBtn , INPUT);
    
  display.init();
  display.flipScreenVertically();
  stateController->SetState(new StartState( stateController ) , "StartState"  );


}

void loop() {
  // put your main code here, to run repeatedly:


  stateController->StateUpdate();
  
  delay(16);

}


void StartState::StateEnd(){

     _stateController->StateFirstRun  = true;
}

 void StartState::StateUpdate(){

   CurrentTime = millis();



    /*切換State*/

    if(CurrentTime - LastTime > _StartStateDefine.StartStateOverMillSec){
      _stateController->SetState( new MainMenuState( _stateController) , "MainMenuState" );
    }


    
}

void MainMenuState::StateEnd(){

     _stateController->StateFirstRun  = true;
}


void MainMenuState::StateUpdate(){
      _index = BtnChoice(_index);
        CursorOffset();
        Draw();

       /*切換State*/


       if(  digitalRead(ConfirmBtn)  == HIGH ){

          _stateController->SetState(new MusicState( _stateController , _index) , "MusicState"  );

       }
}

void MusicState::StateEnd(){

     _stateController->StateFirstRun  = true;
     ledcWriteTone(Channel, 0); 

}


void MusicState::StateUpdate(){

    display.clear();
   CurrentTime = millis();
      CurrentTimeHit = millis();
      _SongCurrentTime = millis();
      TextTimeCurrent = millis();
     
     if(_SongCurrentTime -  _SongLastTime > SongStartTime[_index]){
        PlayMusic();
     }
     else{
        LastTime = CurrentTime;
     }

      DrawBack();
      Draw();
      
      display.display();
       /*切換State*/

      if(_SongIndexBuzzer  >= _SongLength){
           _stateController->SetState(new CalculateState(_stateController,Score , _index) , "CalculateState" );
      }
       
       
}

void CalculateState::StateEnd(){

     _stateController->StateFirstRun  = true;

}


void CalculateState::StateUpdate(){
  CurrentTime = millis();



    /*切換State*/

    if(CurrentTime - LastTime > _ClaCulateStateDefine.StartStateOverMillSec && digitalRead(ConfirmBtn) == HIGH){

      _stateController->SetState( new StartState( _stateController ) , "StartState" );

    }


}


IState::IState(StateController* stateController ) {_stateController =  stateController;}
