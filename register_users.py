import argparse
import json
import requests

parser = argparse.ArgumentParser()
parser.add_argument('--file', help='Path to JSON files with user data', required=True)
parser.add_argument('--url', help='URL of the auth service', required=True)

args = parser.parse_args()

with open(args.file, 'r') as f:
    users = json.load(f)['objects']

results = dict()

for user in users:
    resp = requests.post(args.url, data=user)

    if resp.status_code not in results:
        results[resp.status_code] = 0

    results[resp.status_code] += 1

    login = user['login']

    if resp.status_code != 201:
        print(f'Status code {resp.status_code} for user with login {login}')

print('Results by response code:')
print(results)