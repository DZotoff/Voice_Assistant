import requests

url = "https://westeurope.api.cognitive.microsoft.com/sts/v1.0/issuetoken"
API_key = "525387d68976477fb09f1438dfccc3bc"

headers = {'content-type':'application/yang-data+json',"Ocp-Apim-Subscription-Key": API_key}

response = requests.post(url, headers=headers)
print(response)
access_token = response.text
print(access_token)


headers_to_post_audio = {   
    "Ocp-Apim-Subscription-Key": API_key,
    "content-type": "audio/wav",
    'Authorization': 'Bearer '+access_token
}
print()
url_to_post = "https://westeurope.stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1?language=en-US"
audio_file = open("test_sample.wav","rb")

response = requests.post(url_to_post, headers = headers_to_post_audio, data=audio_file)
data = response.json()
print(data["DisplayText"])
audio_file.close()