import React, {useState} from 'react';
import { AnyRecordWithTtl } from 'dns';
import axios from 'axios';
import "./App.css";
import mqtt from "mqtt"


const myStorage:any = {
  setItem: (key:any, item:any) => {
    myStorage[key] = item;
  },
  getItem: (key:any) => myStorage[key],
  removeItem: (key:any) => {
    delete myStorage[key];
  },
};
var client  = mqtt.connect("ws://iot.eclipse.org/ws");
//<script src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.min.js" type="text/javascript"></script>
//const client = new Client({ uri: 'ws://iot.eclipse.org/ws', clientId: 'clientId', storage: myStorage });
//client.connect();

enum COMMAND  {
  FORWARD ,
  REVERSE,
  CW,
  CCW
};

interface ICommandInfo
{
  icon: string,
  text: string
}

var CommandInfo: ICommandInfo[] = [];
CommandInfo[COMMAND.FORWARD]= { icon: "icons/forward.png", text:"Forward" };
CommandInfo[COMMAND.REVERSE]= { icon: "icons/reverse.png", text:"Reverse" };
CommandInfo[COMMAND.CW]= { icon: "icons/cw.png", text:"CW" };
CommandInfo[COMMAND.CCW]= { icon: "icons/ccw.png", text:"CCW" };


interface ICardProps {
  id : number,
  command: COMMAND,
  name : string,
  company:string,
  duration: any,
  handleDurationChange : any
}


const Card = (props: ICardProps) => {

 const handleChange =  (event:any) => {
   props.handleDurationChange(props.id , event.target.value);
  }
	  	return (
      <div className="commandBox">
    	  <img src={CommandInfo[props.command].icon} />
        <input type="range"
          min="0"
          max="10"
          step="0.01"
          value={props.duration} 
          onChange={ handleChange } />
        
        <div className="company">{props.duration}</div>
          <div className="name">{CommandInfo[props.command].text}</div>  
      </div>
      
    );
}

interface IFormProps {
  addCommand:any
}

class AddCommandPanel extends React.Component<IFormProps> {
  

  addForward = (event:any) => {
    this.props.addCommand(COMMAND.FORWARD);
   };
 
   addReverse = (event:any) => {
    this.props.addCommand(COMMAND.REVERSE);
   };
 
   addCW = (event:any) => {
    this.props.addCommand(COMMAND.CW);
   };
 
   addCCW = (event:any) => {
    this.props.addCommand(COMMAND.CCW);
   };
   
   testSending = (event:any) => {
    
    //const client = new MQTT.Client("ws://iot.eclipse.org/ws", "myClientId" + new Date().getTime());
    if (client.disconnected === true) {
        alert("Not connected to mqtt server");

    }

    const message =  (`{
      "player":"Playername",
      "name":"Carname",
      "msgid":303,
      "pwd": "pwdString", "cmd": [
     { "cmdId": 1, "leftMotor": 1,"rightMotor": 1, "duration_ms":  1000},
     { "cmdId": 2, "leftMotor": -1,"rightMotor": 1, "duration_ms":  325},
     { "cmdId": 3, "leftMotor": 1,"rightMotor": 1, "duration_ms":  1000},
     { "cmdId": 4, "leftMotor": -1,"rightMotor": -1, "duration_ms":  3000},
     { "cmdId": 5, "leftMotor": -1,"rightMotor": 1, "duration_ms":  3000},
     { "cmdId": 6, "leftMotor": 1,"rightMotor": 1, "duration_ms":  2000}
     ]
      }`);
        client.publish('roboderby/robotIn/command', message);
      
   // message.destinationName = 'roboderby/robotIn/command';
   // client.send(message);


   };
 

	render() {
  	return (
      <div className="addCommand">
        <button onClick={this.testSending}>Test</button>
        
        <img src={CommandInfo[COMMAND.FORWARD].icon} onClick={this.addForward} />
        <img src={CommandInfo[COMMAND.REVERSE].icon} onClick={this.addReverse} />
        <img src={CommandInfo[COMMAND.CW].icon} onClick={this.addCW} />
        <img src={CommandInfo[COMMAND.CCW].icon} onClick={this.addCCW} />
      </div>
    );
  }
}

interface IState {
  command: ICardProps[]
}

interface IAppProps{
  title:string
}

const App = (props: any ) => {

  const [commands, setCommands] = useState<ICardProps[]>([]);
  const [duration, setDuration] = useState(4);

  const changeDuration = (id: number , newDuration:number) => {
    console.log("Duration change");
    console.log(id);
    console.log(newDuration); 
    let index = commands.findIndex((e: ICardProps) => e.id === id);
    console.log(index);
    console.log(commands.length);
    console.log(duration);
    setDuration(newDuration);
    if (index >= 0) {
      let newCommands = [... commands];
      newCommands[index].duration = newDuration;
      setCommands(newCommands);
    }
  }

  const addCommand = (commandType:COMMAND) => {    
    let newCmd : ICardProps = 
    {
      id : new Date().getTime(),
      command: commandType,
      name : "forward",
      company: "Company",
      duration : duration,
      handleDurationChange : changeDuration
    }
    console.log(commands.length);
  	setCommands( [...commands, newCmd])
  };


  	return (
    	<div>
    	  <div className="header">{props.title}</div>
        <AddCommandPanel addCommand={addCommand} />
        <div>
  	  {commands.map( (properties:ICardProps) => <Card key={properties.id} {... properties} handleDurationChange={changeDuration}/>)}
	    </div>
    	</div>
    );
  }	



export default App;