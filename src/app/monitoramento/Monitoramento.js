'use client';
import React, { useEffect, useState } from 'react';
import { db } from '../../services/firebase';
import { ref, onValue } from 'firebase/database';
import { Card, CardContent, Grid, Typography } from '@mui/material';
import ThermostatIcon from '@mui/icons-material/Thermostat';
import AirIcon from '@mui/icons-material/Air';

import led_green from '../assets/led_green.png';
import led_red from '../assets/led_red.png';
import Image from 'next/image';

import './Monitoramento.css';

export default function Monitoramento() {

	const [dadosPlaca, setDadosPlaca] = useState({led: 1, temperatura: '23.5', umidade: '62'});
	const [corLed, setCorLed] = useState('');
	const [nomeCor, setNomeCor] = useState('');
	const [descricaoCor, setDescricaoCor] = useState('');
	const [descricaoTemperatura, setDescricaoTemperatura] = useState('');
	const [descricaoUmidade, setDescricaoUmidade] = useState('');

	function getPlaca() {
		const distanciaRef = ref(db, '/esp');

		return onValue(distanciaRef, (placa) => {
			if (placa.exists()) {
				setDadosPlaca(placa.val());
			}
		});
	}

	useEffect(() => {

		getPlaca();

	}, []);

	useEffect(() => {

		/*
			LED
			0 - Ligado (Fixo)
			1 - Piscando
		*/

		if (dadosPlaca.led === 1) {
			setCorLed(led_red)
			setNomeCor('piscando')
			setDescricaoCor('Atenção! A temperatura ou umidade esta fora do ideal, favor verificar a sala!!')
		} else if (dadosPlaca.led === 0) {
			setCorLed(led_green)
			setNomeCor('ligado')
			setDescricaoCor('A sala está com a temperatura ideal.')
		}

	}, [dadosPlaca.led]);

	useEffect(() => {
		if (dadosPlaca.temperatura < 22) {
			setDescricaoTemperatura('Temperatura abaixo do esperado, favor verificar!')
		} else if (dadosPlaca.temperatura > 26) {
			setDescricaoTemperatura('Temperatura acima do esperado, favor verificar!')
		} else {
			setDescricaoTemperatura('Temperatura ideal.')
		}
	}, [dadosPlaca.temperatura]);

	useEffect(() => {
		if(dadosPlaca.umidade < 50) {
			setDescricaoUmidade('Umidade abaixo do ideal, favor verificar a sala!')
		} else if (dadosPlaca.umidade > 80) {
			setDescricaoUmidade('Umidade acima do ideal, favor verificar a sala!')
		} else {
			setDescricaoUmidade('Umidade ideal.')
		}
	}, [dadosPlaca.umidade]);

	return (
		<div>
			<Grid container spacing={3}>

				<Grid item xs={12} md={4} lg={4}>
					<Card sx={{ maxWidth: 345 }}>
						{corLed !== '' ?
							<div className='imageLed'>
								<Image src={corLed} width={40} height={40} alt="Cor do Led" />
							</div>
							: ''}
						<CardContent>
							<Typography gutterBottom variant="h5" component="div">
								Led {nomeCor}
							</Typography>
							<Typography variant="body2" color="text.secondary">
								{descricaoCor}
							</Typography>
						</CardContent>
					</Card>
				</Grid>

				<Grid item xs={12} md={4} lg={4}>
					<Card sx={{ maxWidth: 345 }}>
						<CardContent>
							<Typography gutterBottom variant="h5" component="div">
								<ThermostatIcon />Temperatura: {dadosPlaca.temperatura}°C
							</Typography>
							<Typography variant="body2" color="text.secondary">
								{descricaoTemperatura}
							</Typography>
						</CardContent>
					</Card>
				</Grid>

				<Grid item xs={12} md={4} lg={4}>
					<Card sx={{ maxWidth: 345 }}>
						<CardContent>
							<Typography gutterBottom variant="h5" component="div">
								<AirIcon />Umidade: {dadosPlaca.umidade}%
							</Typography>
							<Typography variant="body2" color="text.secondary">
							{descricaoUmidade}
							</Typography>
						</CardContent>
					</Card>
				</Grid>

			</Grid>

		</div>
	);
}