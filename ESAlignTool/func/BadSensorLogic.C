int ESAlignTool::BadSensor(int iz,int ip,int ix,int iy)
{
 int res=0;
 if(  (iz==1&&ip==1&&ix==24&&iy==33)
    ||(iz==1&&ip==1&&ix==25&&iy==33)
    ||(iz==1&&ip==1&&ix==16&&iy==15)
    ||(iz==1&&ip==1&&ix==16&&iy==14)
    ||(iz==1&&ip==1&&ix==17&&iy==14)
    ||(iz==1&&ip==1&&ix==16&&iy==12)
    ||(iz==1&&ip==1&&ix==17&&iy==12)
    ||(iz==1&&ip==1&&ix==16&&iy==10)
    ||(iz==1&&ip==1&&ix==17&&iy==10)
    ||(iz==1&&ip==1&&ix==25&&iy==26)
    ||(iz==1&&ip==1&&ix==24&&iy==28)
    ||(iz==1&&ip==1&&ix==25&&iy==28)
    ||(iz==1&&ip==1&&ix==24&&iy==27)
    ||(iz==1&&ip==1&&ix==25&&iy==27)
    ||(iz==1&&ip==1&&ix==24&&iy==30)
    ||(iz==1&&ip==1&&ix==25&&iy==30)
    ||(iz==1&&ip==2&&ix==27&&iy==16)
    ||(iz==1&&ip==2&&ix==27&&iy==17)
    ||(iz==1&&ip==1&&ix==14&&iy==13)
    ||(iz==1&&ip==1&&ix==15&&iy==13)
    ||(iz==1&&ip==2&&ix==23&&iy==14)
    ||(iz==1&&ip==1&&ix==6&&iy==28)
    ||(iz==1&&ip==1&&ix==7&&iy==28)
    ||(iz==1&&ip==1&&ix==16&&iy==27)
    ||(iz==1&&ip==1&&ix==17&&iy==27)
    ||(iz==1&&ip==1&&ix==16&&iy==30)
    ||(iz==1&&ip==1&&ix==17&&iy==30)
    ||(iz==1&&ip==1&&ix==16&&iy==29)
    ||(iz==1&&ip==1&&ix==17&&iy==29)
    ||(iz==1&&ip==1&&ix==16&&iy==8)
    ||(iz==1&&ip==1&&ix==17&&iy==8)
    ||(iz==1&&ip==1&&ix==16&&iy==9)
    ||(iz==1&&ip==1&&ix==17&&iy==9)
    ||(iz==1&&ip==1&&ix==16&&iy==6)
    ||(iz==1&&ip==1&&ix==17&&iy==6)
    ||(iz==1&&ip==1&&ix==16&&iy==7)
    ||(iz==1&&ip==1&&ix==17&&iy==7)
    ||(iz==1&&ip==1&&ix==24&&iy==31)
    ||(iz==1&&ip==1&&ix==25&&iy==31)
    ||(iz==1&&ip==1&&ix==24&&iy==32)
    ||(iz==1&&ip==1&&ix==25&&iy==32)
    ||(iz==1&&ip==1&&ix==24&&iy==35)
    ||(iz==1&&ip==1&&ix==25&&iy==35)
    ||(iz==1&&ip==1&&ix==24&&iy==34)
    ||(iz==1&&ip==1&&ix==25&&iy==34)
    ||(iz==1&&ip==1&&ix==33&&iy==22)
    ||(iz==1&&ip==1&&ix==25&&iy==33)
    ||(iz==1&&ip==1&&ix==6&&iy==26)
    ||(iz==1&&ip==1&&ix==14&&iy==13)
    ||(iz==1&&ip==1&&ix==15&&iy==13)
    ||(iz==1&&ip==1&&ix==14&&iy==16)
    
    ||(iz==1&&ip==2&&ix==19&&iy==38)
    ||(iz==1&&ip==2&&ix==20&&iy==30)
    ||(iz==1&&ip==2&&ix==11&&iy==20)
    ||(iz==1&&ip==2&&ix==10&&iy==21)
    ||(iz==1&&ip==2&&ix==10&&iy==20)
    ||(iz==1&&ip==2&&ix==11&&iy==21)
    ||(iz==1&&ip==2&&ix==12&&iy==10)
    ||(iz==1&&ip==2&&ix==33&&iy==7)
    ||(iz==1&&ip==2&&ix==33&&iy==6)
    ||(iz==1&&ip==2&&ix==34&&iy==7)
    ||(iz==1&&ip==2&&ix==23&&iy==14)
    ||(iz==1&&ip==2&&ix==27&&iy==16)
    ||(iz==1&&ip==2&&ix==27&&iy==17)
    ||(iz==1&&ip==2&&ix==29&&iy==21)
    
    ||(iz==-1&&ip==1&&ix==25&&iy==26)
    ||(iz==-1&&ip==1&&ix==25&&iy==27)
    ||(iz==-1&&ip==1&&ix==24&&iy==27)
    ||(iz==-1&&ip==1&&ix==25&&iy==28)
    ||(iz==-1&&ip==1&&ix==24&&iy==28)
    ||(iz==-1&&ip==1&&ix==25&&iy==15)
    ||(iz==-1&&ip==1&&ix==25&&iy==13)
    ||(iz==-1&&ip==1&&ix==24&&iy==13)
    ||(iz==-1&&ip==1&&ix==25&&iy==14)
    ||(iz==-1&&ip==1&&ix==24&&iy==14)
    ||(iz==-1&&ip==1&&ix==25&&iy==12)
    ||(iz==-1&&ip==1&&ix==24&&iy==12)
    ||(iz==-1&&ip==1&&ix==14&&iy==23)
    ||(iz==-1&&ip==1&&ix==35&&iy==20)
    ||(iz==-1&&ip==1&&ix==34&&iy==20)
    ||(iz==-1&&ip==1&&ix==9&&iy==20)
    ||(iz==-1&&ip==1&&ix==8&&iy==20)
    ||(iz==-1&&ip==1&&ix==25&&iy==30)
    ||(iz==-1&&ip==1&&ix==24&&iy==30)
    ||(iz==-1&&ip==1&&ix==25&&iy==29)
    ||(iz==-1&&ip==1&&ix==24&&iy==29)
    ||(iz==-1&&ip==1&&ix==16&&iy==36)
    ||(iz==-1&&ip==1&&ix==23&&iy==11)
    ||(iz==-1&&ip==1&&ix==27&&iy==12)
    
    ||(iz==-1&&ip==2&&ix==28&&iy==22)
    ||(iz==-1&&ip==2&&ix==28&&iy==23)
    ||(iz==-1&&ip==2&&ix==27&&iy==23)
    ||(iz==-1&&ip==2&&ix==30&&iy==22)
    ||(iz==-1&&ip==2&&ix==30&&iy==23)
    ||(iz==-1&&ip==2&&ix==29&&iy==22)
    ||(iz==-1&&ip==2&&ix==29&&iy==23)
    ||(iz==-1&&ip==2&&ix==31&&iy==22)
    ||(iz==-1&&ip==2&&ix==31&&iy==23)
    ||(iz==-1&&ip==2&&ix==33&&iy==22)
    ||(iz==-1&&ip==2&&ix==33&&iy==23)
    ||(iz==-1&&ip==2&&ix==32&&iy==22)
    ||(iz==-1&&ip==2&&ix==32&&iy==23)
    ||(iz==-1&&ip==2&&ix==5&&iy==24)
    ||(iz==-1&&ip==2&&ix==3&&iy==12)
    ||(iz==-1&&ip==2&&ix==19&&iy==6)
    ||(iz==-1&&ip==2&&ix==24&&iy==14)
    ||(iz==-1&&ip==2&&ix==29&&iy==12)
    ||(iz==-1&&ip==2&&ix==35&&iy==17)
   )
  res=1;

 return res;
}

