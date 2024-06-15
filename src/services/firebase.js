'use client'
import { initializeApp } from 'firebase/app';
import { getDatabase } from 'firebase/database';

const firebaseConfig = {
  apiKey: "AIzaSyCJzobgUQPZ-nRpICq-V2o_1U6JDL6_kNc",
  authDomain: "projeto-iot-unesc.firebaseapp.com",
  databaseURL: "https://projeto-iot-unesc-default-rtdb.firebaseio.com",
  projectId: "projeto-iot-unesc",
  storageBucket: "projeto-iot-unesc.appspot.com",
  messagingSenderId: "112714080764",
  appId: "1:112714080764:web:27b4a39b003bdb08c11279"
};

const fire = initializeApp(firebaseConfig);
const db = getDatabase(fire);

export { fire, db };