import React, { useState } from 'react';
import {
  View,
  Text,
  TouchableOpacity,
  StyleSheet,
  Platform,
  //PlatformColor,
  OpaqueColorValue,
  Alert,
  ScrollView,
  useWindowDimensions,
  TextInput,
} from 'react-native';
import Paho from './lib/Paho';
import storage from './services/storage/inMemoryStorage';
import { LineChart } from 'react-native-gifted-charts';

storage;

interface history {
  number_of_items: number;
  max_lenght: number;
  values: number[];
}

const client = new Paho.Client('192.168.1.2', 9001, '', Date.now().toString());

const App = () => {
  const [latestData, setLatestData] = useState([{ value: 0 }]);
  const [latestData1, setLatestData1] = useState([{ value: 0 }]);
  const [latestData2, setLatestData2] = useState([{ value: 0 }]);
  const [newva, setnew] = useState<number>();
  const [show, setshow] = useState(false);
  const [uptime, setuptime] = useState<number>();
  const { width } = useWindowDimensions();

  let color: string | OpaqueColorValue = styles.container.backgroundColor;
  if (Platform.OS === 'windows') {
    color = 'transparent';
    //color = PlatformColor('AcrylicBackgroundFillColorDefaultBrush');
  }

  client.onMessageArrived = (messge: {
    payloadString: string;
    topic: string;
  }) => {
    //console.log('Mensagem recebida: \n\n');
    //console.log(messge);
    //console.log(messge?.topic);
    console.log(messge?.payloadString);
    if (messge?.topic === 'analogic/history') {
      const receivedMSG: string | undefined = messge?.payloadString;

      if (receivedMSG === undefined) {
        return;
      }
      //console.log('msg recebida não é undef\n');
      //JSON.parse(receivedMSG);

      const val: history = JSON.parse(receivedMSG);

      //console.log('\n\nmorreu???\n\n');
      //console.log(val);

      if (val?.values && Array.isArray(val.values)) {
        const historico: { value: number }[] = [];
        for (let i = 0; i < val.values.length; i++) {
          historico.push({ value: val.values[i] });
        }
        //console.log('last state\n');
        //console.log(latestData);
        setLatestData(historico);
      }
    }
    if (messge?.topic === '5/history') {
      const receivedMSG: string | undefined = messge?.payloadString;

      if (receivedMSG === undefined) {
        return;
      }
      //console.log('msg recebida não é undef\n');
      //JSON.parse(receivedMSG);

      const val: history = JSON.parse(receivedMSG);

      //console.log('\n\nmorreu???\n\n');
      //console.log(val);

      if (val?.values && Array.isArray(val.values)) {
        const historico: { value: number }[] = [];
        for (let i = 0; i < val.values.length; i++) {
          historico.push({ value: val.values[i] });
        }
        //console.log('last state\n');
        //console.log(latestData);
        setLatestData1(historico);
      }
    }
    if (messge?.topic === 'update_interval') {
      const receivedMSG: string | undefined = messge?.payloadString;

      if (receivedMSG === undefined) {
        return;
      }
      const upds = Number(receivedMSG);
      if (upds !== NaN) {
        setuptime(upds);
      }
    }
    if (messge?.topic === '16/history') {
      const receivedMSG: string | undefined = messge?.payloadString;

      if (receivedMSG === undefined) {
        return;
      }
      //console.log('msg recebida não é undef\n');
      //JSON.parse(receivedMSG);

      const val: history = JSON.parse(receivedMSG);

      //console.log('\n\nmorreu???\n\n');
      //console.log(val);

      if (val?.values && Array.isArray(val.values)) {
        const historico: { value: number }[] = [];
        for (let i = 0; i < val.values.length; i++) {
          historico.push({ value: val.values[i] });
        }
        //console.log('last state\n');
        //console.log(latestData);
        setLatestData2(historico);
      }
    }
    if (messge?.payloadString) {
      //setMessages(prev => [...prev, messge?.payloadString]);
    }
  };

  const unusableArea = 35 + 40 + 36;

  return (
    <ScrollView style={[styles.container, { backgroundColor: color }]}>
      <View style={{ padding: 20, backgroundColor: 'red' }}>
        <Text>
          Velocidade de atualização: {uptime ? uptime * 6 : uptime} ms
        </Text>
      </View>
      {show && (
        <View>
          <TextInput
            placeholder="Tempo em milissegundos"
            onChangeText={text => {
              console.log(text);
              const nv = Number(text);
              setnew(isNaN(nv) ? undefined : nv);
            }}
            value={newva ? newva + '' : undefined}
            style={{ padding: 10, borderWidth: 1, maxWidth: 200 }}
          />
          <View style={{ flexDirection: 'row' }}>
            <TouchableOpacity
              style={styles.button}
              //disabled={client.isConnected()}
              onPress={() => {
                setshow(false);
                client.publish('nwUP', newva + '', 2, false);
                setnew(undefined);
              }}>
              <Text>Salvar</Text>
            </TouchableOpacity>
            <TouchableOpacity
              style={styles.button}
              //disabled={client.isConnected()}
              onPress={() => {
                setshow(false);
                setnew(undefined);
              }}>
              <Text>Cancelar</Text>
            </TouchableOpacity>
          </View>
        </View>
      )}
      {true && (
        <TouchableOpacity
          style={styles.button}
          //disabled={client.isConnected()}
          onPress={() => setshow(true)}>
          <Text>Atualizar velocidade</Text>
        </TouchableOpacity>
      )}
      <View
        style={{
          backgroundColor: '#FFFFFF3F',
          padding: 20,
          borderRadius: 10,
          borderWidth: 1,
          borderColor: 'rgba(0, 0, 0, 0.05)',
          marginBottom: 20,
        }}>
        {/*
         * Historico de ananlogico
         */}
        <Text>Hitorico de sensor analogico:</Text>
        <LineChart
          areaChart
          //yAxisLabelWidth={35}
          animateOnDataChange={false}
          isAnimated={true}
          animationDuration={1000}
          onDataChangeAnimationDuration={300}
          data={latestData}
          showVerticalLines={true}
          width={width - unusableArea}
          hideDataPoints
          spacing={(width - unusableArea) / 8.84}
          disableScroll={true}
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
          pointerConfig={
            Platform.OS === 'windows'
              ? undefined
              : {
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
                          <Text
                            style={{ fontWeight: 'bold', textAlign: 'center' }}>
                            {'$' + items[0].value + '.0'}
                          </Text>
                        </View>
                      </View>
                    );
                  },
                }
          }
          adjustToWidth={true}
        />
      </View>
      <View
        style={{
          backgroundColor: '#FFFFFF3F',
          padding: 20,
          borderRadius: 10,
          borderWidth: 1,
          borderColor: 'rgba(0, 0, 0, 0.05)',
          marginBottom: 20,
        }}>
        {/*
         * Historico de D1
         */}
        <Text>Hitorico de sensor digital D1:</Text>
        <LineChart
          areaChart
          //yAxisLabelWidth={35}
          animateOnDataChange={false}
          isAnimated={true}
          animationDuration={1000}
          onDataChangeAnimationDuration={300}
          data={latestData1}
          //showVerticalLines={true}
          width={width - unusableArea}
          hideDataPoints
          spacing={(width - unusableArea) / 8.84}
          disableScroll={true}
          color="#00ff83"
          thickness={2}
          startFillColor="rgba(20,105,81,0.3)"
          endFillColor="rgba(20,85,81,0.01)"
          startOpacity={0.9}
          endOpacity={0.2}
          initialSpacing={0}
          noOfSections={2}
          stepValue={1}
          maxValue={2}
          yAxisColor="white"
          yAxisThickness={0}
          //rulesType="solid"
          //rulesColor="gray"
          yAxisTextStyle={{ color: 'gray' }}
          //yAxisSide='right'
          xAxisColor="blue"
          pointerConfig={
            Platform.OS === 'windows'
              ? undefined
              : {
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
                          <Text
                            style={{ fontWeight: 'bold', textAlign: 'center' }}>
                            {'$' + items[0].value + '.0'}
                          </Text>
                        </View>
                      </View>
                    );
                  },
                }
          }
          adjustToWidth={true}
        />
      </View>
      <View
        style={{
          backgroundColor: '#FFFFFF3F',
          padding: 20,
          borderRadius: 10,
          borderWidth: 1,
          borderColor: 'rgba(0, 0, 0, 0.05)',
          marginBottom: 20,
        }}>
        {/*
         * Historico de ananlogico
         */}
        <Text>Hitorico de sensor digital D0:</Text>
        <LineChart
          areaChart
          //yAxisLabelWidth={35}
          animateOnDataChange={false}
          isAnimated={true}
          animationDuration={1000}
          onDataChangeAnimationDuration={300}
          data={latestData2}
          //showVerticalLines={true}
          width={width - unusableArea}
          hideDataPoints
          spacing={(width - unusableArea) / 8.84}
          disableScroll={true}
          color="#00ff83"
          thickness={2}
          startFillColor="rgba(20,105,81,0.3)"
          endFillColor="rgba(20,85,81,0.01)"
          startOpacity={0.9}
          endOpacity={0.2}
          initialSpacing={0}
          noOfSections={2}
          maxValue={2}
          yAxisColor="white"
          yAxisThickness={0}
          //rulesType="solid"
          //rulesColor="gray"
          yAxisTextStyle={{ color: 'gray' }}
          //yAxisSide='right'
          xAxisColor="blue"
          pointerConfig={
            Platform.OS === 'windows'
              ? undefined
              : {
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
                          <Text
                            style={{ fontWeight: 'bold', textAlign: 'center' }}>
                            {'$' + items[0].value + '.0'}
                          </Text>
                        </View>
                      </View>
                    );
                  },
                }
          }
          adjustToWidth={true}
        />
      </View>
      <TouchableOpacity
        style={styles.button}
        disabled={client.isConnected()}
        onPress={() => {
          if (client.isConnected()) {
            Alert.alert('Ja tá conectado');
            return;
          }
          client.connect({
            userName: 'aluno',
            password: '@luno*123',
            onFailure: f => console.log('Failure', f),
            onSuccess: () => {
              console.log('Sucesso');
              client.subscribe('analogic/history', {
                onSuccess: d => {
                  console.log('incrito em analogic/history');
                },
              });
              client.subscribe('5/history', {
                onSuccess: d => {
                  console.log('incrito em 5/history');
                },
              });
              client.subscribe('16/history', {
                onSuccess: d => {
                  console.log('incrito em 16/history');
                },
              });
              client.subscribe('update_interval', {
                onSuccess: d => {
                  console.log('incrito em update_interval');
                },
              });
            },
            reconnect: true,
          });
        }}>
        <Text>{client.isConnected() ? 'Conectado ✅' : 'Conectar 📡'}</Text>
      </TouchableOpacity>
      {/*}
      <TouchableOpacity
        disabled={!client.isConnected()}
        onPress={() => {
          console.log('count + 1');
          client.send('Temp', 'from: ' + Platform.OS);
        }}
        style={styles.button}>
        <Text>Log me!</Text>
      </TouchableOpacity>
      {*/}
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#F3F3F3',
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
