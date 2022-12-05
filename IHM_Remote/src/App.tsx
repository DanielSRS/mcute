import React, { useState } from 'react';
import {
  View,
  Text,
  TouchableOpacity,
  StyleSheet,
  Platform,
  PlatformColor,
  OpaqueColorValue,
  useColorScheme,
  Alert,
  ScrollView,
} from 'react-native';
import Paho from './lib/Paho';
import storage from './services/storage/inMemoryStorage';

storage;

const client = new Paho.Client('192.168.1.2', 9001, '', Date.now().toString());

const App = () => {
  const [count, setCount] = useState(0);
  //const client = useMemo(() => , []);
  const colorScheme = useColorScheme();
  const [messages, setMessages] = useState<string[]>([]);

  let color: string | OpaqueColorValue = 'transparent';
  if (Platform.OS === 'windows' && true) {
    color = PlatformColor('AcrylicBackgroundFillColorDefaultBrush');
  }

  client.onMessageArrived = messge => {
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

  return (
    <View style={[styles.container, { backgroundColor: color }]}>
      <Text style={styles.title}>
        Hello from {'\n'}React Native Web! {`:${colorScheme}`}
      </Text>
      <TouchableOpacity
        onPress={() => setCount(count + 1)}
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
