<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.8" tiledversion="1.8.0" name="tileset" tilewidth="32" tileheight="32" tilecount="256" columns="16">
 <image source="tileset.png" width="512" height="512"/>
 <tile id="8">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="0" width="32" height="32"/>
  </objectgroup>
 </tile>
 <wangsets>
  <wangset name="Dirt" type="mixed" tile="-1">
   <wangcolor name="" color="#ff0000" tile="-1" probability="1"/>
  </wangset>
  <wangset name="Grass" type="mixed" tile="-1">
   <wangcolor name="" color="#ff0000" tile="-1" probability="1"/>
   <wangcolor name="" color="#00ff00" tile="-1" probability="1"/>
   <wangtile tileid="0" wangid="1,1,2,2,2,1,1,1"/>
   <wangtile tileid="1" wangid="1,1,2,2,2,2,2,1"/>
   <wangtile tileid="2" wangid="1,1,1,1,2,2,2,1"/>
   <wangtile tileid="3" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="4" wangid="2,2,2,1,2,2,2,2"/>
   <wangtile tileid="5" wangid="2,2,2,1,1,1,2,2"/>
   <wangtile tileid="6" wangid="2,2,2,2,2,1,2,2"/>
   <wangtile tileid="16" wangid="2,2,2,2,2,1,1,1"/>
   <wangtile tileid="17" wangid="2,2,2,2,2,2,2,2"/>
   <wangtile tileid="18" wangid="2,1,1,1,2,2,2,2"/>
   <wangtile tileid="20" wangid="2,1,1,1,2,2,2,2"/>
   <wangtile tileid="21" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="22" wangid="2,2,2,2,2,1,1,1"/>
   <wangtile tileid="32" wangid="2,2,2,1,1,1,1,1"/>
   <wangtile tileid="33" wangid="2,2,2,1,1,1,2,2"/>
   <wangtile tileid="34" wangid="2,1,1,1,1,1,2,2"/>
   <wangtile tileid="36" wangid="2,1,2,2,2,2,2,2"/>
   <wangtile tileid="37" wangid="1,1,2,2,2,2,2,1"/>
   <wangtile tileid="38" wangid="2,2,2,2,2,2,2,1"/>
  </wangset>
 </wangsets>
</tileset>
