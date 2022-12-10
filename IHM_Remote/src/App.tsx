import React, { useState } from 'react';
import {
  View,
  Text,
  TouchableOpacity,
  StyleSheet,
  Platform,
  PlatformColor,
  OpaqueColorValue,
  Alert,
  ScrollView,
  useWindowDimensions,
} from 'react-native';
import Paho from './lib/Paho';
import storage from './services/storage/inMemoryStorage';
import { LineChart } from 'react-native-gifted-charts';

storage;

const client = new Paho.Client('10.0.0.101', 9001, '', Date.now().toString());

const App = () => {
  const [count, setCount] = useState(0);
  const [latestData, setLatestData] = useState([
    { value: 16, date: '1 Apr 2022', label: '' },
    { value: 180, date: '2 Apr 2022', label: '' },
    { value: 190, date: '3 Apr 2022', label: '' },
    { value: 180, date: '4 Apr 2022', label: '' },
    { value: 140, date: '5 Apr 2022', label: '' },
    {
      value: 240,
      date: '10 Apr 2022',
      label: '',
      labelTextStyle: { color: 'lightgray', width: 60 },
    },
    { value: 0, date: '7 Apr 2022', label: '' },
    { value: 0, date: '8 Apr 2022', label: '' },
    { value: 0, date: '7 Apr 2022', label: '' },
    { value: 1000, date: '8 Apr 2022', label: '' },
  ]);
  //const client = useMemo(() => , []);
  const [messages, setMessages] = useState<string[]>([]);
  const { width } = useWindowDimensions();

  let color: string | OpaqueColorValue = 'transparent';
  if (Platform.OS === 'windows' && false) {
    color = PlatformColor('AcrylicBackgroundFillColorDefaultBrush');
  }

  client.onMessageArrived = (messge: { payloadString: string }) => {
    console.log('Mensagem recebida: \n\n');
    //console.log(messge);
    //console.log(messge?.topic);
    //console.log(messge?.payloadString);
    if (messge?.payloadString) {
      setMessages(prev => [...prev, messge?.payloadString]);
    }
  };

  const msgReceived = messages.map((val, index) => (
    <Text key={index + ''}>{val}</Text>
  ));

  const unusableArea = 35 + 40 + 36;

  return (
    <View style={[styles.container, { backgroundColor: color }]}>
      <View
        style={{ backgroundColor: '#0000003F', padding: 20, borderRadius: 10 }}>
        <LineChart
          areaChart
          //yAxisLabelWidth={35}
          animateOnDataChange={false}
          isAnimated={true}
          animationDuration={1000}
          onDataChangeAnimationDuration={300}
          data={latestData}
          //showVerticalLines={true}
          width={width - unusableArea}
          hideDataPoints
          spacing={(width - unusableArea) / 8.84}
          //disableScroll={true}
          color="#00ff83"
          thickness={2}
          startFillColor="rgba(20,105,81,0.3)"
          endFillColor="rgba(20,85,81,0.01)"
          startOpacity={0.9}
          endOpacity={0.2}
          initialSpacing={0}
          noOfSections={7}
          maxValue={1024}
          yAxisColor="white"
          yAxisThickness={0}
          //rulesType="solid"
          //rulesColor="gray"
          yAxisTextStyle={{ color: 'gray' }}
          //yAxisSide='right'
          xAxisColor="blue"
          pointerConfig={{
            pointerStripHeight: 160,
            pointerStripColor: 'lightgray',
            pointerStripWidth: 2,
            pointerColor: 'lightgray',
            radius: 6,
            pointerLabelWidth: 100,
            pointerLabelHeight: 90,
            activatePointersOnLongPress: true,
            autoAdjustPointerLabelPosition: false,
            pointerLabelComponent: items => {
              return (
                <View
                  style={{
                    height: 90,
                    width: 100,
                    justifyContent: 'center',
                    marginTop: -30,
                    marginLeft: -40,
                  }}>
                  <Text
                    style={{
                      color: 'white',
                      fontSize: 14,
                      marginBottom: 6,
                      textAlign: 'center',
                    }}>
                    {items[0].date}
                  </Text>

                  <View
                    style={{
                      paddingHorizontal: 14,
                      paddingVertical: 6,
                      borderRadius: 16,
                      backgroundColor: 'white',
                    }}>
                    <Text style={{ fontWeight: 'bold', textAlign: 'center' }}>
                      {'$' + items[0].value + '.0'}
                    </Text>
                  </View>
                </View>
              );
            },
          }}
          adjustToWidth={true}
        />
      </View>
      <TouchableOpacity
        onPress={() => {
          setCount(count + 10);
          setLatestData(prev => {
            let f = [...prev];
            f[0].value = prev[0].value + 10;
            return f;
          });
        }}
        style={styles.button}>
        <Text>Click me!</Text>
      </TouchableOpacity>
      <TouchableOpacity
        style={styles.button}
        onPress={() => {
          if (client.isConnected()) {
            Alert.alert('Ja tá conectado cara de bagre');
            return;
          }
          client.connect({
            userName: 'aluno',
            password: '@luno*123',
            onFailure: f => console.log('Failure', f),
            onSuccess: () => {
              console.log('Sucesso');
              client.subscribe('/swd/', {
                onSuccess: d => {
                  console.log('incrito em /swd/');
                },
              });
            },
            reconnect: true,
          });
        }}>
        <Text>Conectar!</Text>
      </TouchableOpacity>
      <TouchableOpacity
        disabled={!client.isConnected()}
        onPress={() => {
          console.log('count + 1');
          client.send('/swd/', 'from: ' + Platform.OS);
        }}
        style={styles.button}>
        <Text>Log me!</Text>
      </TouchableOpacity>

      <Text>You clicked {count} times!</Text>
      <ScrollView>{msgReceived}</ScrollView>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#C3E8BD',
    paddingTop: 40,
    paddingHorizontal: 10,
  },
  button: {
    backgroundColor: '#ADBDFF',
    padding: 5,
    marginVertical: 20,
    alignSelf: 'flex-start',
  },
  title: {
    fontSize: 40,
  },
});

export default App;
